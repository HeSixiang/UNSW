#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <addrspace.h>
#include <vm.h>
#include <machine/tlb.h>

#include <proc.h>
#include <spl.h>
#include <elf.h>
#include <machine/tlb.h>


/* Place your page table functions here */
// Modify kern/vm/vm.c to insert , lookup, and 
// update page table entries, and keep the TLB consistent with the page table.

void vm_insert(paddr_t ** pagetable, uint32_t entry_hi, uint32_t rw) {
    KASSERT(pagetable != NULL);
    uint32_t first_index = F_INDEX(entry_hi);
    uint32_t second_index = S_INDEX(entry_hi);
    paddr_t * second_pagetable = pagetable[first_index];
    if (second_pagetable == NULL) {
        // create an empty page for second_level page table
        second_pagetable = kmalloc(sizeof(paddr_t) * VM_NUM_PAGE);
        KASSERT(second_pagetable != NULL);
        for (int i = 0; i < VM_NUM_PAGE; i++) {
            second_pagetable[i] = PTE_EMPTY;
        }
    }
    KASSERT(second_pagetable[second_index] == PTE_EMPTY);
    pagetable[first_index] = second_pagetable;
    vaddr_t new_page = alloc_kpages(1);
    bzero((void*)new_page, PAGE_SIZE); // Zero-fill
    second_pagetable[second_index] = (KVADDR_TO_PADDR(new_page) & PAGE_FRAME) | TLBLO_VALID | rw;
    KASSERT(second_pagetable[second_index] != PTE_EMPTY);
}

paddr_t vm_lookup(paddr_t ** pagetable, uint32_t entry_hi) {
    KASSERT(pagetable != NULL);
    uint32_t first_index = F_INDEX(entry_hi);
    uint32_t second_index = S_INDEX(entry_hi);
    if (pagetable[first_index] == NULL) return PTE_EMPTY;
    return pagetable[first_index][second_index];
}

// not used
void vm_update(paddr_t ** pagetable, uint32_t entry_hi, bool rw) {
    KASSERT(pagetable != NULL);
    uint32_t first_index = F_INDEX(entry_hi);
    uint32_t second_index = S_INDEX(entry_hi);
    KASSERT(pagetable[first_index][second_index] != PTE_EMPTY);
    if (rw) {
        // set dirty bit to 1
        pagetable[first_index][second_index] |= TLBLO_VALID;
    } else {
        pagetable[first_index][second_index] &= ~TLBLO_VALID;
    }
}

void vm_bootstrap(void)
{
    /* Initialise any global components of your VM sub-system here.  
     *  
     * You may or may not need to add anything here depending what's
     * provided or required by the assignment spec.
     */
}

int
vm_fault(int faulttype, vaddr_t faultaddress)
{
    if (faulttype != VM_FAULT_READ && faulttype != VM_FAULT_WRITE && faulttype != VM_FAULT_READONLY) {
        return EINVAL;
        
    }

    if (faulttype == VM_FAULT_READONLY) return EFAULT;
    
    struct addrspace *as = proc_getas();
    KASSERT(as != NULL);
    KASSERT(as->pagetable != NULL);

    // lookup PT
    paddr_t physical_address = vm_lookup(as->pagetable, faultaddress & PAGE_FRAME);
    // if valid Translation, load TLB
    if (physical_address != PTE_EMPTY) {
        int spl = splhigh(); 
        tlb_random(faultaddress & TLBHI_VPAGE, physical_address | as->loading_mode); 
        splx(spl);
        return 0;
    }

    // look up region
    struct region * target_region = NULL;
    struct region * current_region = as->region;
    while(current_region != NULL) {
        if (faultaddress < current_region->as_vbase) {
            current_region = current_region->next;
            continue;
        }
        if (faultaddress >= current_region->as_vbase + current_region->memsize) {
            current_region = current_region->next;
            continue;
        }
        target_region = current_region; //find it!
        break;
    }
    if (target_region == NULL) return EFAULT;

    // Allocate Frame, Zero-fill, Insert PTE
    uint32_t permission = target_region->p_flag;
    if ((permission & PF_W) != 0) {
        vm_insert(as->pagetable, faultaddress & PAGE_FRAME, TLBLO_DIRTY);
    } else {
        vm_insert(as->pagetable, faultaddress & PAGE_FRAME, 0);
    }
    physical_address = vm_lookup(as->pagetable, faultaddress & PAGE_FRAME);
    KASSERT(physical_address != PTE_EMPTY);
    int spl = splhigh(); 
    tlb_random(faultaddress & TLBHI_VPAGE, physical_address | as->loading_mode); 
    splx(spl);
    return 0;
}

/*
 * SMP-specific functions.  Unused in our UNSW configuration.
 */

void
vm_tlbshootdown(const struct tlbshootdown *ts)
{
	(void)ts;
	panic("vm tried to do tlb shootdown?!\n");
}

