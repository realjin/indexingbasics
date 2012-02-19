#ifndef _INDEXINGBASICS_DOCCOL_
#define _INDEXINGBASICS_DOCCOL_

#include <alisttpl.h>
/*
typedef struct _doccol	{
	int size;
} doccol;
*/

typedef struct _doc {
	__u32 did;
	//mmm: other members
} doc;

alisttpl_struct(doc);

typedef struct _doccol	{
	//meta data, etc.
	//actual doc col
	doc_alist* docs;
}doccol;

#endif
