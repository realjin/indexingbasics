#ifndef _INDEXINGBASICS_INDEX_
#define _INDEXINGBASICS_INDEX_

#include "common.h"

#include "alisttpl.h"

/*-----------------------*
 *  Inverted index decl  *
 *-----------------------*/
typedef struct _posting	{
	uint docid;
	uint tf;
} posting;
alisttpl_struct(posting);
typedef struct _invindex_entry {
	uint termid;
	posting_alist* postings;
} invindex_entry;
alisttpl_struct(invindex_entry);

typedef invindex_entry_alist invindex;

/*-----------------------*
 *  Document index decl  *
 *-----------------------*/
typedef struct _terminfo	{
	uint termid;
	uint tf;
} terminfo;
alisttpl_struct(terminfo);

typedef struct _docindex_entry	{
	int docid;
	terminfo_alist* tinfos;
} docindex_entry;
alisttpl_struct(docindex_entry);

typedef docindex_entry_alist docindex;

/*-----------------------*
 *  Inverted index op    *
 *-----------------------*/
invindex* create_invindex();
posting_alist* get_postings(invindex* ii, uint termid);
uint get_tf_from_postings(posting_alist* postings, uint docid);
uint get_tf_from_ii(invindex* ii, uint docid, uint termid, posting_alist** postings);
int add_tf_to_ii(invindex* ii, uint docid, uint termid, uint tf);

#endif
