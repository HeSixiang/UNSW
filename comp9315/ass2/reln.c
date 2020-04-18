// reln.c ... functions on Relations
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "reln.h"
#include "page.h"
#include "tuple.h"
#include "tsig.h"
#include "bits.h"
#include "hash.h"

#include "psig.h"

// open a file with a specified suffix
// - always open for both reading and writing

File openFile(char *name, char *suffix)
{
	char fname[MAXFILENAME];
	sprintf(fname,"%s.%s",name,suffix);
	File f = open(fname,O_RDWR|O_CREAT,0644);
	assert(f >= 0);
	return f;
}

// create a new relation (five files)
// data file has one empty data page

Status newRelation(char *name, Count nattrs, float pF,
                   Count tk, Count tm, Count pm, Count bm)
{
	Reln r = malloc(sizeof(RelnRep));
	RelnParams *p = &(r->params);
	assert(r != NULL);
	p->nattrs = nattrs;
	p->pF = pF,
	p->tupsize = 28 + 7*(nattrs-2);
	Count available = (PAGESIZE-sizeof(Count));
	p->tupPP = available/p->tupsize;
	p->tk = tk; 
	if (tm%8 > 0) tm += 8-(tm%8); // round up to byte size
	p->tm = tm; p->tsigSize = tm/8; p->tsigPP = available/(tm/8);
	if (pm%8 > 0) pm += 8-(pm%8); // round up to byte size
	p->pm = pm; p->psigSize = pm/8; p->psigPP = available/(pm/8);
	if (p->psigPP < 2) { free(r); return -1; }
	if (bm%8 > 0) bm += 8-(bm%8); // round up to byte size
	p->bm = bm; p->bsigSize = bm/8; p->bsigPP = available/(bm/8);
	if (p->bsigPP < 2) { free(r); return -1; }
	r->infof = openFile(name,"info");
	r->dataf = openFile(name,"data");
	r->tsigf = openFile(name,"tsig");
	r->psigf = openFile(name,"psig");
	r->bsigf = openFile(name,"bsig");
	addPage(r->dataf); p->npages = 1; p->ntups = 0;
	addPage(r->tsigf); p->tsigNpages = 1; p->ntsigs = 0;
	addPage(r->psigf); p->psigNpages = 1; p->npsigs = 0;
	addPage(r->bsigf); p->bsigNpages = 1; p->nbsigs = 0;

	// Create a file containing "pm" all-zeroes bit-strings,
    // each of which has length "bm" bits
	// Note: p = param, r = reln
	Count length = p->pm;             // width of page signature (#bits)
	Count width = p->bm;             // width of bit-slice (=maxpages)
	Count max_per_page = p->bsigPP;  // max bit-slices per page
	// Then we know the total size of bsig file pm * bm = length * width
	Bits container = newBits(width); // create all-zeros bit strings.
	PageID pid = p->bsigNpages-1; // pid should be 0
	assert(pid == 0);
	Page current_page = getPage(r->bsigf, pid);
	for (int i = 0; i < length; i++) {
		// check if room on last page;
		if (pageNitems(current_page) == max_per_page) {
			// if not, create new page and write the old page into file
			putPage(r->bsigf, pid, current_page);
			addPage(r->bsigf);
			p->bsigNpages++; // number of bsig pages
			pid++;
			free(current_page);
			current_page = newPage();
			if (current_page == NULL) return -1;;
		}
		putBits(current_page, pageNitems(current_page), container);
		addOneItem(current_page); // page->nitems++; 
		p->nbsigs++;     // number of bit-slice signatures (bsigs)
	}
	// write the rest into file
	putPage(r->bsigf, pid, current_page);
	freeBits(container);
	closeRelation(r);
	return 0;
}

// check whether a relation already exists

Bool existsRelation(char *name)
{
	char fname[MAXFILENAME];
	sprintf(fname,"%s.info",name);
	File f = open(fname,O_RDONLY);
	if (f < 0)
		return FALSE;
	else {
		close(f);
		return TRUE;
	}
}

// set up a relation descriptor from relation name
// open files, reads information from rel.info

Reln openRelation(char *name)
{
	Reln r = malloc(sizeof(RelnRep));
	assert(r != NULL);
	r->infof = openFile(name,"info");
	r->dataf = openFile(name,"data");
	r->tsigf = openFile(name,"tsig");
	r->psigf = openFile(name,"psig");
	r->bsigf = openFile(name,"bsig");
	read(r->infof, &(r->params), sizeof(RelnParams));
	return r;
}

// release files and descriptor for an open relation
// copy latest information to .info file
// note: we don't write ChoiceVector since it doesn't change

void closeRelation(Reln r)
{
	// make sure updated global data is put in info file
	lseek(r->infof, 0, SEEK_SET);
	int n = write(r->infof, &(r->params), sizeof(RelnParams));
	assert(n == sizeof(RelnParams));
	close(r->infof); close(r->dataf);
	close(r->tsigf); close(r->psigf); close(r->bsigf);
	free(r);
}

// insert a new tuple into a relation
// returns page where inserted
// returns NO_PAGE if insert fails completely

PageID addToRelation(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL && strlen(t) == tupSize(r));
	Page p;  PageID pid;
	RelnParams *rp = &(r->params);
	Bool craete_new_tupe_page = FALSE;
	
	// add tuple to last page
	pid = rp->npages-1;
	p = getPage(r->dataf, pid);
	// check if room on last page; if not add new page
	if (pageNitems(p) == rp->tupPP) {
		addPage(r->dataf);
		rp->npages++;
		pid++;
		free(p);
		p = newPage();
		if (p == NULL) return NO_PAGE;
		craete_new_tupe_page = TRUE;
	}
	addTupleToPage(r, p, t);
	rp->ntups++;  //written to disk in closeRelation()
	putPage(r->dataf, pid, p);

	// compute tuple signature and add to tsigf
	// create the signature
	Bits tsig = makeTupleSig(r, t);
	pid = rp->tsigNpages - 1; // number of tsig pages
	p = getPage(r->tsigf, pid);  // handle on tuple signature file
	
	// check if room on last page; if not add new page
	if (pageNitems(p) == rp->tsigPP) {  // max tuple signatures per page
		addPage(r->tsigf);
		rp->tsigNpages++; // number of tsig pages
		pid++;
		free(p);
		p = newPage();
		if (p == NULL) return NO_PAGE;
	}
	putBits(p, pageNitems(p), tsig);
	addOneItem(p); // p->nitems++; 
	rp->ntsigs++;     // number of tuple signatures (tsigs)
	putPage(r->tsigf, pid, p);
	freeBits(tsig);

	// compute page signature and add to psigf
	Bits psig = makePageSig(r, t);
	pid = rp->psigNpages - 1; // number of psig pages
	p = getPage(r->psigf, pid); // handle on page signature file

	// if creating new tupe page or it is the first one
	if (craete_new_tupe_page || pageNitems(p) == 0) {
		// add new page signature
		// check if room on last page; if not add new page
		if (pageNitems(p) == rp->psigPP) {  // max page signatures per page
			addPage(r->psigf);
			rp->psigNpages++; // number of psig pages
			pid++;
			free(p);
			p = newPage();
			if (p == NULL) return NO_PAGE;
		}
		putBits(p, pageNitems(p), psig);
		addOneItem(p); // p->nitems++; 
		rp->npsigs++;     // number of page signatures (psigs)
		putPage(r->psigf, pid, p);	
	} else {
		// update the last page signature
		// create a container
		Bits container = newBits(psigBits(r)); // width of page signature (#bits)
		getBits(p, pageNitems(p) - 1, container);
		orBits(container, psig);
		putBits(p, pageNitems(p) - 1, container);
		freeBits(container);
		putPage(r->psigf, pid, p);
	}
	//freeBits(psig);

	// use page signature to update bit-slices
	// PID = data page where new Tuple inserted
	pid = rp->npages-1;
	// we have already created Psig = makePageSig(Tuple)
	Count length = rp->pm;           // width of page signature (#bits)
	Count width = rp->bm;             // width of bit-slice (=maxpages)
	Count max_per_page = rp->bsigPP;  // max bit-slices per page

	Bits slice = newBits(width);  // create a container

	// for each i in  0..pm-1 
	for (Count i = 0; i < length; i++) {
		// if (Psig bit[i] is 1) 
		if (bitIsSet(psig, i)) {
			PageID target_page = i / max_per_page;
			p = getPage(r->bsigf, target_page);
			Offset target_offset = i % max_per_page;
			// Slice = get i'th bit slice from bsigFile
			getBits(p, target_offset, slice);
			// set the PID'th bit in Slice
			setBit(slice, pid);
			// write updated Slice back to bsigFile
			putBits(p, target_offset, slice);
			putPage(r->bsigf, target_page, p);
			// pointer being freed was not allocated
			// No idea why ... TODO 
			// free(p);
		}
	}
	freeBits(slice);
	freeBits(psig);

	return nPages(r)-1;
}

// displays info about open Reln (for debugging)

void relationStats(Reln r)
{
	RelnParams *p = &(r->params);
	printf("Global Info:\n");
	printf("Dynamic:\n");
    printf("  #items:  tuples: %d  tsigs: %d  psigs: %d  bsigs: %d\n",
			p->ntups, p->ntsigs, p->npsigs, p->nbsigs);
    printf("  #pages:  tuples: %d  tsigs: %d  psigs: %d  bsigs: %d\n",
			p->npages, p->tsigNpages, p->psigNpages, p->bsigNpages);
	printf("Static:\n");
    printf("  tups   #attrs: %d  size: %d bytes  max/page: %d\n",
			p->nattrs, p->tupsize, p->tupPP);
	printf("  sigs   bits/attr: %d\n", p->tk);
	printf("  tsigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->tm, p->tsigSize, p->tsigPP);
	printf("  psigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->pm, p->psigSize, p->psigPP);
	printf("  bsigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->bm, p->bsigSize, p->bsigPP);
}
