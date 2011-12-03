#include "index.h"

/*-----------------------*
 *  Inverted index op    *
 *-----------------------*/

alisttpl_struct_impl(posting);
alisttpl_struct_impl(ii_term);

alisttpl_struct_impl(di_dterm);
alisttpl_struct_impl(di_doc);

/*---------------------------------------*
 *          Document index op            *
 *---------------------------------------*/

//desc: wrapper method
ii* create_ii()
{
	return create_ii_term_alist();
}

ii_term* ii_create_term()
{
	ii_term* t = (ii_term*)malloc(sizeof(ii_term));
	t->postings = create_posting_alist();
	return t;
}

//return: 0 if failure
posting_alist* get_postings(ii* ii, __u32 tid)
{
	int i;
	for(i=0;i<ii->size;i++)	{
		if(ii->list[i]->tid==tid)	{
			return ii->list[i]->postings;
		}
	}
	return 0;
}

//return: 0 or positve for tf, -1 for result of not found
int get_tf_from_postings(posting_alist* postings, __u32 did)
{
	int i;
	//printf("%d(psize), %u(did)\n",postings->size, did);
	for(i=0;i<postings->size;i++)	{
		if(postings->list[i]->did==did)	{
			return postings->list[i]->tf;
		}
	}
	return -1;
}

//return: 0 or positive for tf, -1 for term entry but no doc entry, -2 for no term entry 
int get_tf_from_ii(ii* ii, __u32 did, __u32 tid, posting_alist** postings)
{
	posting_alist* _postings = get_postings(ii, tid);
	int tf;
	if(!_postings)	{
		//printf("gettf: no term entry!\n");
		return -2;
	}
	else {
		tf = get_tf_from_postings(_postings, did);
		if(tf>=0)	{
			//printf("gettf: term entry and posting exist!did=%u,tid=%u,tf=%u\n", did, tid, tf);
			return tf;
		}
		else	{
			//printf("gettf: term entry exist but exact posting not!\n");
			*postings = _postings; 
			return -1;
		}
	}
}

ii_term* ii_get_term(ii* ind, __u32 tid)
{
	int i;
	for(i=0;i<ind->size;i++)	{
		if(ind->list[i]->tid == tid)	{
			return ind->list[i];
		}
	}
	return 0;
}

//return: 0 for success, 1 if exist for the same doc
int add_tf_to_ii(ii* ii, __u32 did, __u32 tid, __u32 tf)
{
	ii_term* ie;
	posting_alist* postings;
	posting* p;
	int _tf;

	postings = 0;
	_tf = get_tf_from_ii(ii, did, tid, &postings);
	//printf("_tf=%u, truefalse=%d\n",_tf, (-2>=0));
	if(_tf>=0)	{
		printf("add_tf_to_ii failure! old tf exist: did=%d, tid=%d, tf=%d, old tf=%d\n", did, tid, tf, _tf);
		return 1;
	}
	else {
		if(did%4000<10)	{
			printf("add_tf_to_ii did=%d, tid=%d, tf=%d\n", did, tid, tf);
		}
		p = (posting*)malloc(sizeof(posting));
		p->did = did;
		p->tf = tf;
		if(_tf==-1)	{
			//			printf("new posting, postings=%d\n", postings);
			add_posting(postings, p);
		}
		else	{
			ie = (ii_term*)malloc(sizeof(ii_term));
			ie->tid = tid;

			postings = create_posting_alist();
			//printf("new postings\n");
			add_posting(postings, p);
			ie->postings = postings;

			add_ii_term(ii, ie);
		}
	}
	return 0;
}

void ii_show(ii* ind)
{
	int i,j;
	printf("ind size=%d\n", ind->size);
	for(i=0;i<ind->size;i++)	{
		printf("term #%d : (df=%d)\n", ind->list[i]->tid, ind->list[i]->postings->size);
		for(j=0;j<ind->list[i]->postings->size;j++)	{
			//printf("@doc%d, +%d\n",ind->list[i]->postings->list[j]->did, ind->list[i]->postings->list[j]->tf);
		}
		printf("==================\n");
	}
}



/*---------------------------------------*
 *          Document index op            *
 *---------------------------------------*/
di* create_di()
{
	return create_di_doc_alist();
}

di_dterm* di_create_dterm()
{
	di_dterm* dt = (di_dterm*)malloc(sizeof(di_dterm));
	return dt;
}

di_dterm* di_get_dterm(di_doc* d, __u32 tid)
{
	int i;
	if(!d)	{
		printf("di_get_term error! (doc null)\n");
		return 0;
	}

	for(i=0;i<d->terms->size;i++)	{
		if(d->terms->list[i]->tid == tid)	{
			return d->terms->list+i;
		}
	}

	//printf("di_get_term error! (doc does not have term info with tid %d)\n", tid);
	return 0;
}

di_doc* di_create_doc()
{
	di_doc* d = (di_doc*)malloc(sizeof(di_doc));
	d->terms = create_di_dterm_alist();
	return d;
}

di_doc* di_get_doc(di* ind, __u32 did)
{
	int i;
	//printf("in get doc did=%d\n",did);
	for(i=0;i<ind->size;i++)	{
		if(ind->list[i]->did == did)	{
			//printf("get doc ok!!! i=%d\n", i);
			return ind->list[i];
		}
	}	

	return 0;
}

/*
	 int di_add_doc(di* ind, di_doc* d)
	 {
	 return add_di_doc(ind, d);
	 }
 */

int add_tf_to_di(di* ind, __u32 did, __u32 tid, __u32 tf)
{
	di_doc* d = di_get_doc(ind, did);
	di_dterm* dt;

#if 1
	if(did%1000<10)	{
		printf("in add_tf_to_di: did = %d, tid = %d\n", did, tid);
	}
#endif

	if(!d)	{
		d = di_create_doc();
		d->did = did;

		dt = di_create_dterm();
		dt->tid = tid;
		dt->tf = tf;

		add_di_dterm(d->terms,dt);

		return add_di_doc(ind, d);
	}
	else	{
		//printf("d exist! ind->list[0]->did=%d, addr=%x\n",ind->list[0]->did, &(ind->list[0]));
		//printf("d exist! d->did=%d, d=%x\n", d->did, d);
		if(!di_get_dterm(d, tid))	{

			dt = di_create_dterm();
			dt->tid = tid;
			dt->tf = tf;

			add_di_dterm(d->terms, dt);

			return 0;
		}
		else	{
			printf("term with tid %d already exist in doc with did %d!\n", tid, did);
			return -1;
		}
	}
}

int di_add_dterm(di_doc* d, di_dterm* dt)
{
	int i;
	for(i=0;i<d->terms->size;i++)	{
		if(d->terms->list[i]->tid == dt->tid)	{
			printf("[di_add_dterm] same term(tid=%d) in doc(did=%d) exist!\n", d->did, dt->tid);
			return -1;
		}
	}
	if(i==d->terms->size)	{
		add_di_dterm(d->terms, dt);
	}

	return 0;
}


/*
	 add doc to di with no duplicate added
 */
int di_add_doc(di* ind, di_doc* d)
{
	int i,j,k,l;
	for(i=0;i<ind->size;i++)	{
		if(ind->list[i]->did == d->did)	{
			break;
		}
	}
	if(i==ind->size)	{
		add_di_doc(ind, d);
	}
	else	{
		for(j=0;j<d->terms->size;j++)	{
			di_add_dterm(ind->list[i], d->terms->list[j]);
		}
	}

	return 0;
}


void di_show(di* ind)
{
	int i,j;
	printf("ind size=%d\n", ind->size);
	for(i=0;i<ind->size;i++)	{
		printf("doc #%d : (valid term kind=%d)\n", ind->list[i]->did, ind->list[i]->terms->size);
		for(j=0;j<ind->list[i]->terms->size;j++)	{
			printf("term %d: +%d\n",ind->list[i]->terms->list[j]->tid, ind->list[i]->terms->list[j]->tf);
		}
		printf("==================\n");
	}
}


/*---------------------------------------*
 *             Full index op             *
 *---------------------------------------*/
fi* fi_create_fi()
{
	fi* ind = (fi*)malloc(sizeof(fi));
	ind->d = create_di();
	ind->i = create_ii();
	ind->docs = (doccol*)malloc(sizeof(doccol));
	ind->terms = (termcol*)malloc(sizeof(termcol));
}

