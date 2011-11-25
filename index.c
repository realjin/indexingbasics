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
uint get_tf_from_postings(posting_alist* postings, uint docid)
{
	int i;
	for(i=0;i<postings->size;i++)	{
		if(postings->list[i]->docid==docid)	{
			return postings->list[i]->tf;
		}
	}
	return -1;
}

//return: 0 or positive for tf, -1 for term entry but no doc entry, -2 for no term entry 
uint get_tf_from_ii(invindex* ii, uint docid, uint termid, posting_alist** postings)
{
	posting_alist* _postings = get_postings(ii, termid);
	uint tf;
	if(!_postings)	{
		return -2;
	}
	else {
		tf = get_tf_from_postings(_postings, docid);
		if(tf>=0)	{
			return tf;
		}
		else	{
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
	uint _tf;

	postings = 0;
	tf = get_tf_from_ii(ii, docid, termid, &postings);
	if(_tf>=0)	{
		printf("add_tf_to_ii failure! old tf exist: docid=%d, termid=%d, old tf=%d\n", docid, termid, tf);
		return 1;
	}
	else {
		p = (posting*)malloc(sizeof(posting));
		p->docid = docid;
		p->tf = tf;
		if(_tf==-1)	{
			add_posting(postings, p);
		}
		else	{
			ie = (invindex_entry*)malloc(sizeof(invindex_entry));
			ie->termid = termid;

			postings = create_posting_alist();
			add_posting(postings, p);

			add_invindex_entry(ii, ie);
		}
	}
	return 0;
}
