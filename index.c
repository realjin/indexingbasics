#include "index.h"

/*-----------------------*
 *  Inverted index op    *
 *-----------------------*/

alisttpl_struct_impl(posting);
alisttpl_struct_impl(invindex_entry);

alisttpl_struct_impl(terminfo);
alisttpl_struct_impl(docindex_entry);

//desc: wrapper method
invindex* create_invindex()
{
	return create_invindex_entry_alist();
}

//return: 0 if failure
posting_alist* get_postings(invindex* ii, uint termid)
{
	int i;
	for(i=0;i<ii->size;i++)	{
		if(ii->list[i]->termid==termid)	{
			return ii->list[i]->postings;
		}
	}
	return 0;
}

//return: 0 or positve for tf, -1 for result of not found
int get_tf_from_postings(posting_alist* postings, uint docid)
{
	int i;
	//printf("%d(psize), %u(docid)\n",postings->size, docid);
	for(i=0;i<postings->size;i++)	{
		if(postings->list[i]->docid==docid)	{
			return postings->list[i]->tf;
		}
	}
	return -1;
}

//return: 0 or positive for tf, -1 for term entry but no doc entry, -2 for no term entry 
int get_tf_from_ii(invindex* ii, uint docid, uint termid, posting_alist** postings)
{
	posting_alist* _postings = get_postings(ii, termid);
	int tf;
	if(!_postings)	{
		//printf("gettf: no term entry!\n");
		return -2;
	}
	else {
		tf = get_tf_from_postings(_postings, docid);
		if(tf>=0)	{
			//printf("gettf: term entry and posting exist!did=%u,tid=%u,tf=%u\n", docid, termid, tf);
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
int add_tf_to_ii(invindex* ii, uint docid, uint termid, uint tf)
{
	invindex_entry* ie;
	posting_alist* postings;
	posting* p;
	int _tf;

	postings = 0;
	_tf = get_tf_from_ii(ii, docid, termid, &postings);
	//printf("_tf=%u, truefalse=%d\n",_tf, (-2>=0));
	if(_tf>=0)	{
		printf("add_tf_to_ii failure! old tf exist: docid=%d, termid=%d, tf=%d, old tf=%d\n", docid, termid, tf, _tf);
		return 1;
	}
	else {
		if(docid%1000<10)	{
		printf("add_tf_to_ii docid=%d, termid=%d, tf=%d\n", docid, termid, tf);
		}
		p = (posting*)malloc(sizeof(posting));
		p->docid = docid;
		p->tf = tf;
		if(_tf==-1)	{
//			printf("new posting, postings=%d\n", postings);
			add_posting(postings, p);
		}
		else	{
			ie = (invindex_entry*)malloc(sizeof(invindex_entry));
			ie->termid = termid;

			postings = create_posting_alist();
			//printf("new postings\n");
			add_posting(postings, p);
			ie->postings = postings;

			add_invindex_entry(ii, ie);
		}
	}
	return 0;
}
