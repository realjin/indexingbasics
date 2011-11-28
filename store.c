#include "store.h"

//srlz: serialize

static iis_term_header* srlz_iiterm(ii_term* t)
{
	iis_term_header* h = (iis_term_header*)malloc(sizeof(iis_term_header));
	h->flag = _INDEXINGBASICS_STORE_FLAG_II_THEADER; 
	h->tid[0] = (t->tid >> 16) & 0xff;
	h->tid[1] = (t->tid >> 8) & 0xff;
	h->tid[2] = (t->tid) & 0xff;
	h->df = t->postings->size;
	return h;
}

static ii_term* desrlz_iisterm_header(iis_term_header* h)
{
	if(h->flag != _INDEXINGBASICS_STORE_FLAG_II_THEADER)	{
		printf("desrlz_iiterm_header error! (flag wrong)\n");
		return 0;
	}
	
	ii_term* t = (ii_term*)malloc(sizeof(ii_term));
	t->tid = h->tid[0]<<16 + h->tid[1]<<8 + h->tid[2];
	t->postings = create_posting_alist();
	return t;
}

static iis_term_body_entry* srlz_iiterm_posting(posting* pst)
{
	iis_term_body_entry* be = (iis_term_body_entry*)malloc(sizeof(iis_term_body_entry));
	be->flag = _INDEXINGBASICS_STORE_FLAG_II_TBODYENTRY; 
	be->did[0] = (pst->did >> 16) & 0xff;
	be->did[1] = (pst->did >> 8) & 0xff;
	be->did[2] = (pst->did ) & 0xff;
	be->tf = pst->tf;
	return be;
}

static posting* desrlz_iisterm_body_entry(iis_term_body_entry* be)
{
	if(be->flag != _INDEXINGBASICS_STORE_FLAG_II_TBODYENTRY)	{
		printf("desrlz_iiterm_body_entry error! (flag wrong)\n");
		return 0;
	}
	posting* p = (posting*)malloc(sizeof(posting));
	p->did = be->did[0]<<16 + be->did[1]<<8 + be->did[2];
	p->tf = be->tf;
	return p;
		
}

int save_ii(ii* ind, char* fn)
{
	FILE* f;
	__u8 end = 0xff;

	int i,j;
	ii_term* t;
	iis_term_header* h;
	iis_term_body_entry* be;
	
	//open------------
	f = fopen(fn, "wb");

	for(i=0;i<ind->size;i++)	{
		t = ind->list[i];
		h = srlz_iiterm(t);
		//write;
		fwrite((void*)(&h->flag), sizeof(__u8), 1, f);
		fwrite((void*)h->tid, sizeof(__u8), 3, f);
		fwrite((void*)(&h->df), sizeof(__u32), 1, f);
		free(h);
		for(j=0;j<t->postings->size;j++)	{
			be = srlz_iiterm_posting(t->postings->list[j]);
			//write;
			fwrite((void*)(&be->flag), sizeof(__u8), 1, f);
			fwrite((void*)be->did, sizeof(__u8), 3, f);
			fwrite((void*)(&be->tf), sizeof(__u32), 1, f);
			free(be);
		}
	}
	fwrite((void*)&end, sizeof(__u8), 1, f);

	fclose(f);

	//close
}

ii* load_ii(char* fn)
{
	FILE* f;

	//open---
	f = fopen(fn, "rb");

	iis_term_header* h;
	iis_term_body_entry* be;
	ii* ind;
	ii_term* t;
	posting* pst;
	int i;

	ind = create_ii_term_alist();

	while(1)	{
		//h = load(8byte);
		h = (iis_term_header*)malloc(sizeof(iis_term_header));
		fread((void*)(&h->flag), sizeof(__u8), 1, f);
		if(h->flag == 0xff)	{	//end of file
			break;
		}
		fread((void*)h->tid, sizeof(__u8), 3, f);
		fread((void*)(&h->df), sizeof(__u32), 1, f);
		t = desrlz_iisterm_header(h);
		for(i=0;i<h->df;i++)	{
			//be = load(8byte);
			fread((void*)(&be->flag), sizeof(__u8), 1, f);
			fread((void*)be->did, sizeof(__u8), 3, f);
			fread((void*)(&be->tf), sizeof(__u32), 1, f);
			be = 0;
			pst = desrlz_iisterm_body_entry(be);
			add_posting(t->postings, pst);
		}
	}

	fclose(f);

	return ind;
}
