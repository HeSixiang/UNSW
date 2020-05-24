// bsig.c ... functions on Tuple Signatures (bsig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"

#include "psig.h"

void findPagesUsingBitSlices(Query q)
{
	assert(q != NULL);

	Reln relation_info = q->rel;
	Tuple target_t = q->qstring; //char * 

	// handle on bit-sliced signature file
	File bsigf = relation_info->bsigf;

	// get dynamic parameters
	Count num_bsig_pages = nBsigPages(relation_info); //number of bsig pages
	//Count num_bsig = nBsigs(relation_info);           // number of bit-sliced signatures (bsigs)
	Count max_bsig = maxBsigsPP(relation_info);       // max bit-sliced signatures per page

	// get fixed parameters (set at relation creation time)
	Count psig_size = psigBits(relation_info);        // width of page signature (#bits)
	Count width = bsigBits(relation_info);            // width of bit-slice (=maxpages)

	//QuerySig = makePageSig(Query)
	Bits query_sig = makePageSig(q->rel, target_t);

	Bits * pages_list = &(q->pages);
	// Pages = AllOneBits
	setAllBits(*pages_list);

	// initialize statistics info
	q->nsigs = 0; 
	q->nsigpages = 0;

	Bits slice = newBits(width);  // create a container
	PageID target_page = num_bsig_pages; // make sure it must be updated
	Page p = NULL;

	// for each i in 0..pm-1
	for (Count i = 0; i < psig_size; i++) {
		// if (Psig bit[i] is 1) 
		if (bitIsSet(query_sig, i)) {
			if (target_page != i / max_bsig) {
				q->nsigpages++;
				target_page = i / max_bsig;
				free(p);
				p = getPage(bsigf, target_page);
			}
			Offset target_offset = i % max_bsig;
			// Slice = get i'th bit slice from bsigFile
			getBits(p, target_offset, slice);
			// zero bits in Pages which are zero in Slice
			// printf("slice:"); showBits(slice); putchar('\n');
			andBits(*pages_list, slice);
			q->nsigs++;
		}
	}
	free(p);
	freeBits(slice);
}

