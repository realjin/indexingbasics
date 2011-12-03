#include "docvector.h"

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
			ii_t = ind->i->list[di_d->terms->list[j]->tid];
			if(ii_t)	{
				//v = tf/df
				v = di_d->terms->list[j]->tf * 1.0 / ii_t->postings->size;
				ret = dv_set_value(dv_d, di_d->terms->list[j]->tid, v);
				if(ret)	{
					printf("dv_create_dv_from_fi error, set value failed(ret code%d): did=%d, tid=%d, v=%f\n", ret, di_d->did, di_d->terms->list[j]->tid, v);
					continue;
				}
				else	{
					add_dv_doc(dv_dlist, dv_d);
				}
			}
			else	{
				//printf("dv_create_dv_from_fi error, t not exist\n");
				continue;
			}	//if(ii_t)
		} //for(j
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
