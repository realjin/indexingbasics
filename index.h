#ifndef _INDEXINGBASICS_INDEX_
#define _INDEXINGBASICS_INDEX_

#include "common.h"

#include <linux/types.h>
#include "alisttpl.h"

//ii: inverted index
//di: document index


/*-----------------------*
 *  Inverted index decl  *
 *-----------------------*/
typedef struct _posting	{
	__u32 did;
	__u32 tf;
} posting;
//rename to ii_posting
alisttpl_struct(posting);
typedef struct _ii_term {
	__u32 tid;
	posting_alist* postings;
} ii_term;
alisttpl_struct(ii_term);

typedef ii_term_alist ii;

/*-----------------------*
 *  Document index decl  *
 *-----------------------*/
typedef struct _terminfo	{
	__u32 tid;
	__u32 tf;
} terminfo;
alisttpl_struct(terminfo);

typedef struct _di_entry	{
	int did;
	terminfo_alist* tinfos;
} di_entry;
alisttpl_struct(di_entry);

typedef di_entry_alist di;

/*-----------------------*
 *  Inverted index op    *
 *-----------------------*/
ii* create_ii();
posting_alist* get_postings(ii* ii, __u32 tid);
int get_tf_from_postings(posting_alist* postings, __u32 did);
int get_tf_from_ii(ii* ii, __u32 did, __u32 tid, posting_alist** postings);
int add_tf_to_ii(ii* ii, __u32 did, __u32 tid, __u32 tf);

#endif
