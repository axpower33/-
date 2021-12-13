#include "array.h"
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define NULL 0L

struct xlong {
	 int lo;
	 int hi;
};

table* makeAr(int x, int y, int s)
{
	 table *t = new table[y+3];
	 ((xlong*)&t[y+1])->lo = x;
	 ((xlong*)&t[y+1])->hi = y;
	 (int)(t[y+2])=s;

	 int i=y; t[y]=NULL;
	 while (i--)
	 t[i]=new char[s*x];
	 return t;
}

void delAr (table* t)
{
	 int i=0;
	 while (t[i++]!=NULL);

	 int l=((xlong*)&t[i])->lo * int(t[i+1]),
	 y=((xlong*)&t[i--])->hi;

	 while (i--) delete[](char*)t[i];

	 delete[]t;
}

