#include "index.h"
#include "reader_nsf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NSF_FOLDER_BOW "../dataset/nsf/bow";
#define NSF_FOLDER_ABST "../dataset/nsf/abstract";

int main()
{
	//invindex* ii;
	//ii = create_invindex_entry_alist();
	//add_tf_to_ii(ii, 12319, 350, 3);

	id_map* idmap;

	doc_terms_alist* dlist;	
	invindex* ii;

	char line[200];
	FILE* f;
	char* fns[150000];
	int n;
	int i;



	f = popen("find ../dataset/nsf/bow -type f -printf '%T@ %p\n' | grep \"idnsfid\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	n = 0;
	while((fgets(line,sizeof(line),f))!=0)	{
		fns[n] = (char*)malloc(sizeof(line));
		memcpy(fns[n], line, strlen(line)-1);
		fns[n][strlen(line)-1]=0;
		//printf("%s\n",fns[n]);
		n++;
	}
	pclose(f);
	idmap = nsf_create_id_map(fns,n);
	printf("final size=%d\n", idmap->size);

	//f = popen("find ../dataset/nsf/abstract -type f -printf '%T@ %p\n' | sed 's/^[^ ]* //'", "r");
	f = popen("find ../dataset/nsf/bow -type f -printf '%T@ %p\n' | grep \"docwords\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	n = 0;
	ii = create_invindex();
	while((fgets(line,sizeof(line),f))!=0)	{
		fns[n] = (char*)malloc(sizeof(line));
		memcpy(fns[n], line, strlen(line)-1);
		fns[n][strlen(line)-1]=0;
		printf("%s\n",fns[n]);
		n++;
		if(n==1)	{
			break;
		}
	}
	pclose(f);
	dlist = nsf_create_doc_terms_list(fns, n);
	printf("---toadd-----dlist size=%d\n", dlist->size);
	for(i=0;i<dlist->size;i++)	{
		add_tf_to_ii(ii,dlist->list[i]->docid, dlist->list[i]->termid, dlist->list[i]->tf);
	}
}
