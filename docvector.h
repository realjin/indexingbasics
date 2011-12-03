#ifndef _INDEXINGBASICS_DOCVECTOR_
#define _INDEXINGBASICS_DOCVECTOR_

#include "common.h"

#include "index.h"
#include <alisttpl.h>

typedef struct _dv_entry	{
	__u32 id;
	double v;
} dv_entry;

alisttpl_struct(dv_entry);
typedef dv_entry_alist dv;


typedef struct _dv_doc	{
	__u32 did;
	dv* vector;
} dv_doc;

alisttpl_struct(dv_doc);
typedef dv_doc_alist dv_docs;




//---api---
dv_doc* dv_create_doc();
int dv_set_value(dv_doc* d, __u32 id, double v);
dv_docs* dv_create_dv_from_fi(fi* ind);


#endif
