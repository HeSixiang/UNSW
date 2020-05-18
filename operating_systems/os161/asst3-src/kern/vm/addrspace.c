/*
 * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <spl.h>
#include <spinlock.h>
#include <current.h>
#include <mips/tlb.h>
#include <addrspace.h>
#include <vm.h>
#include <proc.h>

#include <elf.h>

/*
 * Note! If OPT_DUMBVM is set, as is the case until you start the VM
 * assignment, this file is not compiled or linked or in any way
 * used. The cheesy hack versions in dumbvm.c are used instead.
 *
 * UNSW: If you use ASST3 config as required, then this file forms
 * part of the VM subsystem.
 *
 */

struct addrspace *
as_create(void)
{
	struct addrspace *as;

	as = kmalloc(sizeof(struct addrspace));
	if (as == NULL) return NULL;

	as->region = NULL;
	as->loading_mode = 0;

	paddr_t ** new_page = kmalloc(PAGE_SIZE);
	if (new_page == NULL) {
		kfree(as);
		return NULL;
	}

	for (int i = 0; i < VM_NUM_PAGE; i++) new_page[i] = NULL;
	// kernel memory in kseg0
	as->pagetable = new_page;
	return as;
}

int
as_copy(struct addrspace *old, struct addrspace **ret)
{
	if (old == NULL) return EINVAL;
	struct addrspace *newas;

	newas = as_create();
	if (newas==NULL) return ENOMEM;

	// copy the region
	struct region * old_r = old->region;
	struct region * dummy = kmalloc(sizeof(struct region));
	KASSERT(dummy != NULL);
	dummy->next = NULL;
	struct region * new_r = dummy;
	newas->region = dummy;
	while(old_r != NULL) {
		new_r->next = kmalloc(sizeof(struct region));
		KASSERT(new_r->next != NULL);
		new_r = new_r->next;
		new_r->as_vbase = old_r->as_vbase;
		new_r->memsize = old_r->memsize;
		new_r->p_flag = old_r->p_flag;
		old_r = old_r->next;
	}
	new_r->next = NULL;
	newas->region = dummy->next;
	kfree(dummy);

	// copy the page table
	paddr_t ** new_p = newas->pagetable;
	paddr_t ** old_p = old->pagetable;
	KASSERT(new_p != NULL);
	for (int first_level = 0; first_level < VM_NUM_PAGE; first_level++) {
		if (old_p[first_level] == NULL) continue;
		new_p[first_level] = kmalloc(PAGE_SIZE);
		KASSERT(new_p[first_level] != NULL);
		if (new_p[first_level] == NULL) return ENOMEM;
		for (int second_level = 0; second_level < VM_NUM_PAGE; second_level++) {
			if (old_p[first_level][second_level] == PTE_EMPTY) {
				new_p[first_level][second_level] = PTE_EMPTY;
				continue;
			}
			vaddr_t new_page = alloc_kpages(1);
			if (new_page == 0) return ENOMEM;
			bzero((void*)new_page, PAGE_SIZE); // Zero-fill
			// should not have overlap, memmove
			// Remember, you can access memory through kseg0.
			memcpy((void *)new_page, (void *)PADDR_TO_KVADDR(old_p[first_level][second_level]&PAGE_FRAME), PAGE_SIZE);
			new_p[first_level][second_level] = KVADDR_TO_PADDR(new_page) & PAGE_FRAME;
			// also copy the permisson
			new_p[first_level][second_level] |= (old_p[first_level][second_level] & TLBLO_NOCACHE);
		new_p[first_level][second_level] |= (old_p[first_level][second_level] & TLBLO_DIRTY);
			new_p[first_level][second_level] |= (old_p[first_level][second_level] & TLBLO_VALID);
		}
	}
	*ret = newas;
	return 0;
}

void
as_destroy(struct addrspace *as)
{
	// clean the region
	while(as->region != NULL) {
		struct region * temp = as->region->next;
		kfree(as->region);
		as->region = temp;
	}
	//clean the page table
	paddr_t ** page_table = as->pagetable;
	for (int first_level = 0; first_level < VM_NUM_PAGE; first_level++) {
		if (page_table[first_level] == NULL) continue;
		for (int second_level = 0; second_level < VM_NUM_PAGE; second_level++) {
			if (page_table[first_level][second_level] == PTE_EMPTY) continue;
			free_kpages(PADDR_TO_KVADDR(page_table[first_level][second_level]& PAGE_FRAME));
		}
		kfree(page_table[first_level]);
	}
	kfree(page_table);
	kfree(as);
}

void
as_activate(void)
{
	int i, spl;
	struct addrspace *as;

	as = proc_getas();
	if (as == NULL) {
		/*
		 * Kernel thread without an address space; leave the
		 * prior address space in place.
		 */
		return;
	}

	/* Disable interrupts on this CPU while frobbing the TLB. */
	spl = splhigh();

	for (i=0; i<NUM_TLB; i++) {
		tlb_write(TLBHI_INVALID(i), TLBLO_INVALID(), i);
	}

	splx(spl);
}

void
as_deactivate(void)
{
	/*
	 * Write this. For many designs it won't need to actually do
	 * anything. See proc.c for an explanation of why it (might)
	 * be needed.
	 */
}

/*
 * Set up a segment at virtual address VADDR of size MEMSIZE. The
 * segment in memory extends from VADDR up to (but not including)
 * VADDR+MEMSIZE.
 *
 * The READABLE, WRITEABLE, and EXECUTABLE flags are set if read,
 * write, or execute permission should be set on the segment. At the
 * moment, these are ignored. When you write the VM system, you may
 * want to implement them.
 */

// usually implemented as a linked list of region specifications
int
as_define_region(struct addrspace *as, vaddr_t vaddr, size_t memsize,
		 int readable, int writeable, int executable)
{
	KASSERT(as != NULL);

	struct region * r = kmalloc(sizeof(struct region));
	if (r == NULL) {
		return ENOMEM; 
	}

	// copy from dumbvm, useful~
	/* Align the region. First, the base... */
	memsize += vaddr & ~(vaddr_t)PAGE_FRAME;
	vaddr &= PAGE_FRAME;
	/* ...and now the length. */
	memsize = (memsize + PAGE_SIZE - 1) & PAGE_FRAME;
	KASSERT(memsize % PAGE_SIZE == 0);
	
	r->as_vbase = vaddr;
	r->memsize = memsize;
	r->p_flag = 0;
	if (readable) r->p_flag |= PF_R;
	if (writeable) r->p_flag |= PF_W;
	if (executable) r->p_flag |= PF_X;

	// insert the region 
	r->next = as->region;
	as->region = r;

	return 0;
}

int
as_prepare_load(struct addrspace *as)
{
	as->loading_mode = TLBLO_DIRTY;
	return 0;
}

int
as_complete_load(struct addrspace *as)
{
	as->loading_mode = 0;

	//flush tlb
	/* Disable interrupts on this CPU while frobbing the TLB. */
	int spl = splhigh();

	for (int i=0; i<NUM_TLB; i++) {
		tlb_write(TLBHI_INVALID(i), TLBLO_INVALID(), i);
	}

	splx(spl);
	return 0;
}

int
as_define_stack(struct addrspace *as, vaddr_t *stackptr)
{
	/* Initial user-level stack pointer */
	*stackptr = USERSTACK;
	// You may use a fixed-size stack region (say 16 pages) for each process.
	// The segment in memory extends from VADDR up to (but not including) VADDR+MEMSIZE.
	return as_define_region(as, USERSTACK - (16 * PAGE_SIZE), 16 * PAGE_SIZE, 1, 1, 0);
}

