#include "reader_nsf.h"

#include "alisttpl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


alisttpl_struct_impl(id_map_entry);
alisttpl_struct_impl(doc_terms);

id_map* nsf_create_id_map(char* fns[], int n)
{
	id_map* m = create_id_map_entry_alist();
	int i;
	
	for(i=0;i<n;i++)	{
		nsf_load_bow_idnsfid(m, fns[i]);
	}
	return m;
}


void nsf_load_bow_idnsfid(id_map* m, char* fn)
{
	id_map_entry* e;
	char line[100];
	FILE* f;

	f = fopen(fn, "r");

	while((fgets(line, sizeof(line), f))!=0)	{
		e = (id_map_entry*)malloc(sizeof(id_map_entry));
		sscanf(line, "%d %s", &e->id, &e->docid);

		add_id_map_entry(m, e);
	}

	fclose(f);

	//printf("map size now=%d\n", m->size);
}
//------------------------------------------doc terms
void nsf_load_doc_terms(doc_terms_alist* dlist, char* fn)
{
	doc_terms* d;
	char line[100];
	FILE *f;
	f = fopen(fn, "r");

	while( (fgets(line,sizeof(line), f)) !=0 ){
		d = (doc_terms*)malloc(sizeof(doc_terms));
	//	printf("line=%s\n", line);
		sscanf(line, "%d %d %d", &d->docid, &d->termid, &d->tf);
		add_doc_terms(dlist, d);
	}

	fclose(f);
		

}

doc_terms_alist* nsf_create_doc_terms_list(char* fns[], int n)
{
	doc_terms_alist* dlist = create_doc_terms_alist();
	int i;

	for(i=0;i<n;i++)	{
		nsf_load_doc_terms(dlist, fns[i]);
	}

	return dlist;
	
}
