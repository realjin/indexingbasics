#include "index.h"

/*-----------------------*
 *  Inverted index op    *
 *-----------------------*/

alisttpl_struct_impl(posting);
alisttpl_struct_impl(ii_term);

alisttpl_struct_impl(terminfo);
alisttpl_struct_impl(di_entry);

//desc: wrapper method
ii* create_ii()
{
	return create_ii_term_alist();
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
		if(did%1000<10)	{
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
