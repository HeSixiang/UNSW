// psig.c ... functions on page signatures (psig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include <unistd.h>
#include <string.h>
#include "hash.h"

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"

// exaclly same as codeword in tsig.c 
Bits page_codeword(char *attr_value, Count m, Count k)
{
   int  nbits = 0;   // count of set bits
   Bits cword = newBits(m);
   srandom(hash_any(attr_value, strlen(attr_value)));
   while (nbits < k) {
      int i = random() % m;
      if (!bitIsSet(cword, i)) {
         setBit(cword, i);
         nbits++;
      }
   }
   return cword;  // m-bits with k 1-bits and m-k 0-bits
}

Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	Count m = psigBits(r); // width of page signature (#bits)
	Count k = codeBits(r); // bits set per attribute

	//Psig = AllZeroBits
	Bits psig = newBits(m);

	Count num_att = nAttrs(r); // number of attributes
	char ** list_att = tupleVals(r, t); //get the list of pointers to attributes

	//now, we know have the list of attruibuts and its size
	//for each attribute A in tuple T 
	for (Count i = 0; i < num_att; i++) {
		char * current_att = list_att[i];
		// skip the attribute "?"
		if (strcmp(current_att, "?") == 0) {
			continue;
		}
		//CW = codeword for A
		Bits CW = page_codeword(current_att, m, k);
		//Tsig = Tsig OR CW
		orBits(psig, CW);
		freeBits(CW); //protect memory leak
	}
	return psig;
}

void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);
	// get static info
	Reln relation_info = q->rel; // Relation info
	Tuple target_t = q->qstring; // query string

	// handle on page signature file
	File psigf = relation_info->psigf;

	// get dynamic parameters
	Count num_psig_pages = nPsigPages(relation_info); //number of psig pages
	//Count num_psig = nPsigs(relation_info);           // number of page signatures (psigs)
	Count max_psig = maxPsigsPP(relation_info);       // max page signatures per page

	// get fixed parameters (set at relation creation time)
	Count psig_size = psigBits(relation_info);        // width of page signature (#bits)

	//QuerySig = makePageSig(Query)
	Bits query_sig = makePageSig(q->rel, target_t);

	Bits * pages_list = &(q->pages);
	//Pages = AllZeroBits
	unsetAllBits(*pages_list);

	// initialize statistics info
	q->nsigs = 0; 
	q->nsigpages = 0;

	//foreach Psig in tsigFile
	//loop though each psig pages
	for(PageID pid = 0; pid < num_psig_pages; pid++) {
		Page current_p = getPage(psigf, pid);
		// loop though each page signatures for each psig pages
		for (Count tid = 0; tid < pageNitems(current_p); tid++) {
			// create a contianer for current signatures
			Bits current_sig = newBits(psig_size);
			getBits(current_p, tid, current_sig);
			// if (Tsig matches QuerySig) 
			if (isSubset(query_sig, current_sig)) {
				//PID = data page for tuple corresponding to Psig
				Count position = pid * max_psig + tid;
				//include PID in Pages
				setBit(*pages_list, position);
			}
			q->nsigs++; // update how many signatures read
			freeBits(current_sig);
		}
		q->nsigpages++; //update how many signature pages read
	}

	// The printf below is primarily for debugging
	// Remove it before submitting this function
	// printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}

