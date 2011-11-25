#include "common.h"

#include "alisttpl.h"

typedef struct _posting	{
	uint docid;
	uint tf;
} posting;

alisttpl_struct(posting);

typedef struct _invindex_entry {
	int termid;
	posting_alist* postings;
} invindex_entry;

alisttpl_struct(invindex_entry);

typedef invindex_entry_alist invindex;
	
	

	

