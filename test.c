#include "test.h"

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
