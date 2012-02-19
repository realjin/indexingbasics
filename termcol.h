#ifndef _INDEXINGBASICS_TERMCOL_
#define _INDEXINGBASICS_TERMCOL_


typedef struct _term {
	__u32 tid;
	//mmm: other members
} term;

alisttpl_struct(term);

typedef struct _termcol	{
	//meta data, etc.
	//actual terms
	term_alist* terms;
} termcol;



#endif
