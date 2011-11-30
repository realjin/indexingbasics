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
} posting;	//doc info(related to a term)
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
typedef struct _di_dterm	{
	__u32 tid;
	__u32 tf;
} di_dterm;	//term info (in document)
alisttpl_struct(di_dterm);

typedef struct _di_doc	{
	int did;
	di_dterm_alist* terms;
} di_doc;
alisttpl_struct(di_doc);

typedef di_doc_alist di;

/*-----------------------*
 *  Inverted index op    *
 *-----------------------*/
//two approaches to construction ii:
//1. add <did, tid, tf> tuple
ii* create_ii();
posting_alist* get_postings(ii* ii, __u32 tid);
int get_tf_from_postings(posting_alist* postings, __u32 did);
int get_tf_from_ii(ii* ii, __u32 did, __u32 tid, posting_alist** postings);
int add_tf_to_ii(ii* ii, __u32 did, __u32 tid, __u32 tf);
void ii_show(ii* ind);

/*-----------------------*
 *  Document index op    *
 *-----------------------*/
//two approaches to construction ii:
//1. add <did, tid, tf> tuple
di* create_di();
di_dterm* di_create_dterm();
di_dterm* di_get_dterm(di_doc* d, __u32 tid);
di_doc* di_create_doc();
di_doc* di_get_doc(di* ind, __u32 did);
int di_add_doc(di* ind, di_doc* d);
int add_tf_to_di(di* ind, __u32 did, __u32 tid, __u32 tf);
#endif
