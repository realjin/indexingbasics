#include "reader_nsf.h"

#include "alisttpl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


alisttpl_struct_impl(idmap_entry);


nsf_load_bow_idnsfid(idmap* m, char* fn)
{
	idmap_entry* e;
	char line[100];
	FILE* f;
	int itemp=0;

	f = fopen(fn, "r");

	while((fgets(line, sizeof(line), f))!=0)	{
//		if(itemp++==1000)	{
//			break;
//		}
		e = (idmap_entry*)malloc(sizeof(idmap_entry));
		sscanf(line, "%d %s", &e->id, &e->docid);

		add_idmap_entry(m, e);
	}

	printf("map size now=%d\n", m->size);
}

