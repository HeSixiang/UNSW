// tsig.c ... functions on Tuple Signatures (tsig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "tsig.h"
#include "reln.h"
#include "hash.h"
#include "bits.h"

Bits codeword(char *attr_value, Count m, Count k)
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

// make a tuple signature
Bits makeTupleSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);

	Count m = tsigBits(r); // width of tuple signature (#bits)
	Count k = codeBits(r); // bits set per attribute

	//Tsig = AllZeroBits
	Bits tsig = newBits(m);

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
		Bits CW = codeword(current_att, m, k);
		//Tsig = Tsig OR CW
		orBits(tsig, CW);
		freeBits(CW); //protect memory leak
	}
	return tsig;
}

// find "matching" pages using tuple signatures
void findPagesUsingTupSigs(Query q)
{
	assert(q != NULL);
	// get static info
	Reln relation_info = q->rel; // Relation info
	Tuple target_t = q->qstring; // query string

	// handle on tuple signature file
	File tsigf = relation_info->tsigf;

	// get dynamic parameters
	Count num_tsig_pages = nTsigPages(relation_info); // number of tsig pages
	// Count num_tsig = nTsigs(relation_info);           // number of tuple signatures (tsigs)
	Count max_tsig = maxTsigsPP(relation_info);       // max tuple signatures per page

	// get fixed parameters (set at relation creation time)
	Count tsig_size = tsigBits(relation_info);        // width of tuple signature (#bits)
	Count max_tuple = maxTupsPP(relation_info);       // max tuples per page

	//QuerySig = makeTupleSig(Query)
	Bits query_sig = makeTupleSig(q->rel, target_t);

	Bits * pages_list = &(q->pages);
	//Pages = AllZeroBits
	unsetAllBits(*pages_list);

	// initialize statistics info
	q->nsigs = 0; 
	q->nsigpages = 0;

	//foreach Tsig in tsigFile
	//loop though each tsig pages
	for(PageID pid = 0; pid < num_tsig_pages; pid++) {
		Page current_p = getPage(tsigf, pid);
		// loop though each tuple signatures for each tsig pages
		for (Count tid = 0; tid < pageNitems(current_p); tid++) {
			// getTupleFromPage? NO-> int size = tupSize(r);
			// tuples and tuple signatures are different sizes
			// create a contianer for current signatures
			Bits current_sig = newBits(tsig_size);
			getBits(current_p, tid, current_sig);
			// if (Tsig matches QuerySig) 
			if (isSubset(query_sig, current_sig)) {
				//PID = data page for tuple corresponding to Tsig
				Count position = (pid * max_tsig + tid) / max_tuple;
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
