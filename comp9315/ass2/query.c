// query.c ... query scan functions
// part of SIMC signature files
// Manage creating and using Query objects
// Written by John Shepherd, March 2020

#include "defs.h"
#include "query.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"
#include "tsig.h"
#include "psig.h"
#include "bsig.h"

// check whether a query is valid for a relation
// e.g. same number of attributes

int checkQuery(Reln r, char *q)
{
	if (*q == '\0') return 0;
	char *c;
	int nattr = 1;
	for (c = q; *c != '\0'; c++)
		if (*c == ',') nattr++;
	return (nattr == nAttrs(r));
}

// take a query string (e.g. "1234,?,abc,?")
// set up a QueryRep object for the scan

Query startQuery(Reln r, char *q, char sigs)
{
	Query new = malloc(sizeof(QueryRep));
	assert(new != NULL);
	if (!checkQuery(r,q)) return NULL;
	new->rel = r;
	new->qstring = q;
	new->nsigs = new->nsigpages = 0;
	new->ntuples = new->ntuppages = new->nfalse = 0;
	new->pages = newBits(nPages(r));
	switch (sigs) {
	case 't': findPagesUsingTupSigs(new); break;
	case 'p': findPagesUsingPageSigs(new); break;
	case 'b': findPagesUsingBitSlices(new); break;
	default:  setAllBits(new->pages); break;
	}
	new->curpage = 0;
	return new;
}

// scan through selected pages (q->pages)
// search for matching tuples and show each
// accumulate query stats
void scanAndDisplayMatchingTuples(Query q){
	assert(q != NULL);

	Reln relation_info = q->rel;
	assert(relation_info != NULL);

	// q->rel->params.npages
	Count params_npages = nPages(relation_info);
	File data_file = dataFile(relation_info);

	Tuple target_t = q->qstring; //char * 

	// initialize statistics info
	q->ntuples = 0;
	q->nfalse = 0;
	q->ntuppages = 0;

	// start at page Id 0
	// foreach PID in 0 .. npages-1 {
	for (q->curpage = 0; q->curpage < params_npages; q->curpage++) {
		//if PID is not set in MatchingPages, ignore this page
		if (!bitIsSet(q->pages, q->curpage)) {
			continue;
		}
		
		Page current = getPage(data_file, q->curpage);
		Count num_tuples = pageNitems(current);
		Bool detect_false_match = TRUE;

		//for each tuple T in page PID
		for (q->curtup = 0; q->curtup < num_tuples; q->curtup++) {
			// get data for i'th tuple in Page
			Tuple current_t = getTupleFromPage(relation_info, current, q->curtup);
			//if (T matches the query string)
			if (tupleMatch(relation_info, target_t, current_t)) {
				//display it as a query result
				printf("Matched Pages: %d\n", q->curpage);
				showTuple(relation_info, current_t);
				detect_false_match = FALSE;
			}
			// update how many tuples examined
			q->ntuples++;
		}
		if (detect_false_match) {
			// update how many pages had no matching tuples
			q->nfalse++;
		}
		// update how many data pages read
		q->ntuppages++;
	}
}

// print statistics on query

void queryStats(Query q)
{
	printf("# sig pages read:    %d\n", q->nsigpages);
	printf("# signatures read:   %d\n", q->nsigs);
	printf("# data pages read:   %d\n", q->ntuppages);
	printf("# tuples examined:   %d\n", q->ntuples);
	printf("# false match pages: %d\n", q->nfalse);
}

// clean up a QueryRep object and associated data

void closeQuery(Query q)
{
	free(q->pages);
	free(q);
}

