#include "test.h"

#include "reader_nsf.h"
#include "index.h"
#include "store.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NSF_FOLDER_BOW "../dataset/nsf/bow";
#define NSF_FOLDER_ABST "../dataset/nsf/abstract";


//test di saveing and loading
int main2()
{
	di* tosave = create_di();
	di* loaded;

	add_tf_to_di(tosave, 1, 167, 1);
	add_tf_to_di(tosave, 1, 301, 2);
	add_tf_to_di(tosave, 2, 301, 2);
	add_tf_to_di(tosave, 2, 314, 1);


	printf("----------tosave\n");
	di_show(tosave);

	save_di(tosave, "di.dat");
	loaded = load_di("di.dat");
	
	printf("----------loaded\n");
	di_show(loaded);
}


//test di creation
int main()
{
	di* ind = create_di();

	FILE* f;
	char* fns[150000];
	char line[200];
	int n;
	int i;
	doc_terms_alist* dlist;	

	f = popen("find ../dataset/nsf/bow -type f -printf '%T@ %p\n' | grep \"docwords\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	n = 0;
	while((fgets(line,sizeof(line),f))!=0)	{
		fns[n] = (char*)malloc(sizeof(line));
		memcpy(fns[n], line, strlen(line)-1);
		fns[n][strlen(line)-1]=0;
		printf("%s\n",fns[n]);
		n++;
#if 0
		if(n==1)	{
			break;
		}
#endif
	}
	pclose(f);
	dlist = nsf_create_doc_terms_list(fns, n);
	printf("---toadd-----dlist size=%d\n", dlist->size);

	for(i=0;i<dlist->size;i++)	{
		add_tf_to_di(ind,dlist->list[i]->docid, dlist->list[i]->termid, dlist->list[i]->tf);
	}

	save_di(ind, "full.di");	

}

int main1()
{
	//invindex* ii;
	//ii = create_invindex_entry_alist();
	//add_tf_to_ii(ii, 12319, 350, 3);
	
	id_map* idmap;
	
	char line[200];
	FILE* f;
	char* fns[150000];
	int n;
	
	n = 0;
	f = popen("find ../dataset/nsf/bow -type f -printf '%T@ %p\n' | grep \"idnsfid\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
//	f = popen("find ../dataset/nsf/abstract -type f -printf '%T@ %p\n' | sed 's/^[^ ]* //'", "r");
	while((fgets(line,sizeof(line),f))!=0)	{
		fns[n] = (char*)malloc(sizeof(line));
		memcpy(fns[n], line, strlen(line)-1);
		fns[n][strlen(line)-1]=0;
		printf("%s\n",fns[n]);
		n++;
	}
	idmap = nsf_create_id_map(fns,n);
	printf("final size=%d\n", idmap->size);


}
