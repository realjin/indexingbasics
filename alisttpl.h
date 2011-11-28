#ifndef _AIADT_ALISTTPL_H_
#define _AIADT_ALISTTPL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
	 Array list template emulation
 */

#define NUM_ALIST_INITCAPACITY 100
#define NUM_ALIST_MAXCAPACITY 5*1000*1000
#define NUM_ALIST_INCREMENT 50

#define alisttpl_struct(type)	\
	typedef struct _##type##_alist	{	\
		type** list;	\
		int size;	\
		int capacity;	\
	} type##_alist;	\
\
int expand_##type##_alist(type##_alist* l);	\
int add_##type(type##_alist* l, type* e);	\
int remove_##type(type##_alist* l, int n);

#define alisttpl_struct_impl(type)	\
	type##_alist* create_##type##_alist()	\
{	\
	type##_alist* p = (type##_alist*)malloc(sizeof(type));	\
	p->list = (type**)malloc(NUM_ALIST_INITCAPACITY*(sizeof(type*)));	\
	p->size = 0;	\
	p->capacity = NUM_ALIST_INITCAPACITY;	\
	return p;	\
}	\
int expand_##type##_alist(type##_alist* l)	\
{	\
	int newcapacity = l->capacity+NUM_ALIST_INCREMENT;	\
	type** newlist;	\
	if(newcapacity>NUM_ALIST_MAXCAPACITY)	{	\
		printf("expand alist error(max capacity exceeded).\n");	\
		return -1;	\
	}	\
	else {	\
		newlist = (type**)realloc(l->list, newcapacity*sizeof(type*));	\
		if(newlist==0)	{	\
			printf("expand alist error(realloc failed).\n");	\
			return -1;	\
		}	\
		else	{	\
			l->list = newlist;	\
			l->capacity = newcapacity;	\
			/*size unchanged*/	\
		}	\
	}	\
	return 0;\
}	\
\
int add_##type(type##_alist* l, type* e)	\
{	\
	if(l->size==l->capacity)	{	\
		if(expand_##type##_alist(l)!=0)	{	\
			printf("add element to list error(expand failed).\n");	\
			return -1;	\
		}	\
	}	\
	l->list[l->size] = e;	\
	l->size++;	\
	return 0;	\
}	\
\
int remove_##type(type##_alist* l, int n)	\
{	\
	int i;	\
	if(n<0||n>l->size-1)	{	\
		printf("remove element from list error(invalid index:%d).\n",n);	\
		return -1;	\
	}	\
	else	{	\
		for(i=n;i<l->size-1;i++)	{	\
			l->list[i] = l->list[i+1];	\
		}	\
	}	\
	return 0;	\
}


//add expand list to specified size func ( or create in specific size fun)


#endif
