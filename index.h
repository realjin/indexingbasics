#ifndef _INDEXINGBASICS_INDEX_
#define _INDEXINGBASICS_INDEX_

#include <alisttpl.h>

#include "common.h"

#include "doccol.h"
#include "termcol.h"

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
 *    Full index decl    *
 *-----------------------*/
typedef struct _fi	{
	ii* ii;
	di* di;
	
	doccol* docs;
	termcol* terms;
	
} fi;


/*-----------------------*
 *  Inverted index op    *
 *-----------------------*/
//two approaches to construction ii:
//1. add <did, tid, tf> tuple
ii* create_ii();
int ii_destroy_ii(ii* ind);
ii_term* ii_create_term();	//!!!: "create": only do malloc, no init!!
int ii_destroy_term(ii_term* t);
posting_alist* get_postings(ii* ii, __u32 tid);
int get_tf_from_postings(posting_alist* postings, __u32 did);
int get_tf_from_ii(ii* ii, __u32 did, __u32 tid, posting_alist** postings);
//---api---
ii_term* ii_get_term(ii* ind, __u32 tid);
int add_tf_to_ii(ii* ii, __u32 did, __u32 tid, __u32 tf);
void ii_show(ii* ind);

/*-----------------------*
 *  Document index op    *
 *-----------------------*/
//two approaches to construction ii:
//1. add <did, tid, tf> tuple
//local
di* create_di();
int di_destroy_di(di* ind);
di_dterm* di_create_dterm();
di_dterm* di_get_dterm(di_doc* d, __u32 tid);
di_doc* di_create_doc();
int di_destroy_doc(di_doc* d);
int add_tf_to_di(di* ind, __u32 did, __u32 tid, __u32 tf);
//---api---
int di_add_dterm(di_doc* d, di_dterm* dt);
int di_add_doc(di* ind, di_doc* d);
di_doc* di_get_doc(di* ind, __u32 did);
void di_show(di* ind);

/*-----------------------*
 *     Full index op     *
 *-----------------------*/
//---api---
fi* fi_create_fi();
int fi_destroy_fi(fi* ind);
#endif
