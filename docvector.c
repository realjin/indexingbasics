#include "docvector.h"

#include "store.h"

alisttpl_struct_impl(dv_entry);
alisttpl_struct_impl(dv_doc);

dv_doc* dv_create_doc()
{
	dv_doc* d = (dv_doc*)malloc(sizeof(dv_doc));
	d->vector = create_dv_entry_alist();
	return d;
}

dv_docs* dv_create_dv_from_fi(fi* ind)
{
	dv_docs* dv_dlist;
	int i,j;
	dv_doc* dv_d;
	di_doc* di_d;
	di_dterm* di_dt;
	ii_term* ii_t;
	double v;
	int ret;

	dv_dlist = create_dv_doc_alist();

	printf("dv 1\n");


	for(i=0;i<ind->docs->size;i++) {
#if 1
		if(i%1000==1)	{
			printf("processing doc %d\n", i);
		}
#endif

		//printf("dv 2.1\n");
		dv_d = dv_create_doc();
		dv_d->did = i+1;

		//di_d = di_get_doc(ind->d, i+1);
		di_d = ind->d->list[i];
		//printf("dv 2.2\n");

		for(j=0;j<di_d->terms->size;j++)	{
			//printf("dv 2.3.1\n");
			//ii_t = ii_get_term(ind->i, di_d->terms->list[j]->tid);
			ii_t = ind->i->list[di_d->terms->list[j]->tid-1];
			if(ii_t)	{
				//v = tf/df
				v = di_d->terms->list[j]->tf * 1.0 / ii_t->postings->size;
				//printf("dv v=%f, tid=%d, tf=%d, pstsize=%d\n", v,di_d->terms->list[j]->tid, di_d->terms->list[j]->tf, ii_t->postings->size); 
				//printf("di_d->terms->list[j]->tid=%d, ii_t->tid=%d\n",di_d->terms->list[j]->tid, ii_t->tid);
				ret = dv_set_value(dv_d, di_d->terms->list[j]->tid, v);
				if(ret)	{
					printf("dv_create_dv_from_fi error, set value failed(ret code%d): did=%d, tid=%d, v=%f\n", ret, di_d->did, di_d->terms->list[j]->tid, v);
					continue;
				}
				//printf("set doc %d term %d value %f\n", dv_d->did, di_d->terms->list[j]->tid, v);
			}
			else	{
				//printf("dv_create_dv_from_fi error, t not exist\n");
				continue;
			}	//if(ii_t)
		} //for(j
		add_dv_doc(dv_dlist, dv_d);
	}//for(i

	return dv_dlist;
}

//add if not exist
//replace if exist
//return: 0 if not exist and set, 1 if exist and replaced, -1 if failed
int dv_set_value(dv_doc* d, __u32 id, double v)
{
	int i;
	dv_entry* e;
	for(i=0;i<d->vector->size;i++)	{
		if(d->vector->list[i]->id == id)	{
			d->vector->list[i]->v = v;
			return 1;
		}
	}


	e = (dv_entry*)malloc(sizeof(dv_entry));	
	e->id = id;
	e->v = v;
	if(add_dv_entry(d->vector, e) == 0)	{
		return 0;
	}
	else	{
		return -1;
	}
}

//------------store----------------

dv_docs* dv_load_docs(char* fn)
{
	dv_docs* docs;

	FILE* f;

	dv_doc* d;
	dv_entry* dve;
	__u32 did;
	__u32 id;
	double v;

	int size;	
	int i;

	__u8 b;
	__u8 b3[3];
	__u8 b4[4];
	__u8 b8[8];
	__u64 u64;
	//__u64 u64_rev;
	//double* adouble;

	//int tmp;

	docs = create_dv_doc_alist();

	__u8 end = _INDEXINGBASICS_STORE_END; 

	f = fopen(fn, "rb");

	while(1)	{
		fread((void*)(b4), sizeof(__u8), 4, f);
		if(b4[0]==_INDEXINGBASICS_STORE_END)	{
			break;
		}

		d = dv_create_doc();

		if(b4[0]!=_INDEXINGBASICS_STORE_FLAG_DV_DHEADER)	{
			printf("dv_load_docs error, dheader flag not matched!!!\n");
			//mmm: free(d), etc
			return 0;
		}
		d->did = (b4[1]<<16) + (b4[2]<<8) + b4[3];
		//printf("did=%d %d %d\n", b4[1], b4[2], b4[3]);
#if 1
		if(d->did%1000==1)	{
			printf("loading dv: did = %d\n", d->did);
		}
#endif

		fread((void*)(b4), sizeof(__u8), 4, f);
		size = (b4[0]<<24) + (b4[1]<<16) + (b4[2]<<8) + b4[3];

		for(i=0;i<size;i++)	{
			dve = (dv_entry*)malloc(sizeof(dv_entry));
			
			fread((void*)(&b), sizeof(__u8), 1, f);
			//printf("dbody=%x\n", b);
			if(b!=_INDEXINGBASICS_STORE_FLAG_DV_DBODY)	{
				printf("dv_load_docs error, dbody flag not matched!!!\n");
				return 0;
			}
			fread((void*)(b3), sizeof(__u8), 3, f);
			dve->id = (b3[0]<<16) + (b3[1]<<8) + b3[2];
			fread((void*)(b8), sizeof(__u8), 4, f);	//first half of double

			fread((void*)(&b), sizeof(__u8), 1, f);	//flag duplicate
			fread((void*)(b3), sizeof(__u8), 3, f);	//id duplicate
			fread((void*)(b8+4), sizeof(__u8), 4, f);	//last half of double

			u64 = (((__u64)b8[0])<<56) + (((__u64)b8[1])<<48) + (((__u64)b8[2])<<40) +(((__u64)b8[3])<<32) +(((__u64)b8[4])<<24) +(((__u64)b8[5])<<16) +(((__u64)b8[6])<<8) + (__u64)b8[7]; 
 
			//u64_rev = (b8[7]<<56) + (b8[6]<<48) + (b8[5]<<40) + (b8[4]<<32) + (b8[3]<<24) + (b8[2]<<16) + (b8[1]<<8) + b8[0];
			dve->v = *((double*)&u64);	//mmm: big endian and small endian sensitive?
			//printf("dveval=%f\n", dve->v);

			//printf("\nv====%f, u64=%lx\n", dve->v, u64);

			add_dv_entry(d->vector, dve);
		}

		add_dv_doc(docs, d);
	}

	return docs;

}

int dv_save_docs(dv_docs* docs, char* fn)
{
	int i,j;
	FILE* f;
	dv_doc* d;
	dv_entry* dve;

	__u8 b = 0;
	__u8 b3[3];
	__u8 b4[4];
	__u8 b8[8];

	__u8 end = _INDEXINGBASICS_STORE_END; 

	f = fopen(fn, "wb");

	for(i=0;i<docs->size;i++)	{
		d = docs->list[i];

		b = _INDEXINGBASICS_STORE_FLAG_DV_DHEADER;
		fwrite((void*)&b, sizeof(__u8), 1, f);

		b = (d->did >> 16) & 0xff;
		fwrite((void*)&b, sizeof(__u8), 1, f);
		b = (d->did >> 8) & 0xff;
		fwrite((void*)&b, sizeof(__u8), 1, f);
		b = d->did & 0xff;
		fwrite((void*)&b, sizeof(__u8), 1, f);

		b = (d->vector->size >> 24) & 0xff;
		fwrite((void*)&b, sizeof(__u8), 1, f);
		b = (d->vector->size >> 16) & 0xff;
		fwrite((void*)&b, sizeof(__u8), 1, f);
		b = (d->vector->size >> 8) & 0xff;
		fwrite((void*)&b, sizeof(__u8), 1, f);
		b = d->vector->size & 0xff;
		fwrite((void*)&b, sizeof(__u8), 1, f);

		for(j=0;j<d->vector->size;j++)	{
			dve = d->vector->list[j];

			srlz_double(&dve->v, b8);
			

			b = _INDEXINGBASICS_STORE_FLAG_DV_DBODY;
			fwrite((void*)&b, sizeof(__u8), 1, f);
			b3[0] = (dve->id >> 16) & 0xff;
			b3[1] = (dve->id >> 8) & 0xff;
			b3[2] = dve->id & 0xff;
			fwrite((void*)b3, sizeof(__u8), 3, f);
			fwrite((void*)b8, sizeof(__u8), 4, f);

			fwrite((void*)&b, sizeof(__u8), 1, f);
			fwrite((void*)b3, sizeof(__u8), 3, f);
			fwrite((void*)(b8+4), sizeof(__u8), 4, f);
		}
	}

	fwrite((void*)&end, sizeof(__u8), 1, f);

	fclose(f);
}

