#include "alisttpl.h"


typedef struct _idmap_entry	{
	uint id;
	char docid[20];
}idmap_entry;

alisttpl_struct(idmap_entry);
typedef idmap_entry_alist idmap;

nsf_load_bow_idnsfid(idmap* m, char* fn);


