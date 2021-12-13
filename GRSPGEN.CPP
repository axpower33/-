#include "sgrbox.h"
#include <dos.h>
#include <dir.h>
#include "svgacc.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include "fractal.h"

struct ffblk ffblk;
int done;
int cnt=0;
const Kmx=100;

float Lmin;
float Lmax;
float* Ch;
float* WL;
float* Tm;
float* DF;
float w[15];

float fnMx(float* fun)
{ float Cm;
  Cm=fun[0];
  for (int k=1;k<Kmx;k++)
  if (Cm<fun[k]) Cm=fun[k];
  return Cm;
}

float fnMn(float* fun)
{ float Cm;
  Cm=fun[0];
  for (int k=1;k<Kmx;k++)
  if (Cm>fun[k]) Cm=fun[k];
  return Cm;
}

void DelGrFiles()
{ char fdat[128];
  fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"*.gra");
  done=findfirst(fdat,&ffblk,FA_RDONLY);
  while(!done)
  { fdat[0]=0;
	strcpy(fdat,PathToDat());strcat(fdat,ffblk.ff_name);
	remove(fdat);
	done=findnext(&ffblk);
  }
}

void TrancFrtoL(float *Ch)
{


}

void GrSpGen()
{
  clrscr();
  Ch=new float[100];
  WL=new float[100];
  Tm=new float[Kmx];
  DF=new float[Kmx];

  char fdat[128];
  fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"spectr0.gra");
  done=findfirst(fdat,&ffblk,FA_RDONLY);
  if (!abs(done))
  {  FILE *cl=fopen(fdat,"rt");
	 for(int k=0;k<Kmx;k++)
	 fscanf(cl,"%e",&Ch[k]);
	 fclose(cl);
	 float ChMax=fnMx(Ch);
	 float t;
	 for(k=0;k<Kmx;k++)
	 { Ch[k]/=ChMax;
	   WL[k]=Lmin+k*(Lmax-Lmin)/Kmx;
	 }

	 TrancFrtoL(Ch);
//	 for (k=(Kmx-1)/2,t;t=Ch[k],Ch[k]=Ch[Kmx-k-1],Ch[Kmx-k-1]=t,k--;);

	 float RNG=0.17;
	 InitGraph(Lmin,0,Lmax,1);
	 RangeBox(RNG);
	 Box(4);
	 int clr=0;int cn=0;
	 for (k=1;k<Kmx;k++) Line(RED,WL[k-1],Ch[k-1],WL[k], Ch[k]);

	 fdat[0]=0;
	 strcpy(fdat,PathToDat());strcat(fdat,"*.gra");
	 done=findfirst(fdat,&ffblk,FA_RDONLY);
	 while(!done)
	 { if (strcmp(ffblk.ff_name,"SPECTR0.GRA")==0) done = findnext(&ffblk);
	   fdat[0]=0;
	   strcpy(fdat,PathToDat());strcat(fdat,ffblk.ff_name);
	   FILE *cl=fopen(fdat,"rt");
	   for(int k=0;k<Kmx;k++)
	   { fscanf(cl,"%e",&Tm[k]);
	     Tm[k]/=ChMax;
	     WL[k]=Lmin+k*(Lmax-Lmin)/Kmx;
	   }
	   fclose(cl);
//	   for (k=(Kmx-1)/2,t;t=Tm[k],Tm[k]=Tm[Kmx-k-1],Tm[Kmx-k-1]=t,k--;);

	   clr++;
	   cn++; if (cn==4) cn=1;
	   for (k=1;k<Kmx;k++) Line(cn,WL[k-1], Tm[k-1],WL[k], Tm[k]);

	   for(k=0;k<Kmx;k++) DF[k]=Ch[k]-Tm[k];
	   int Kr;
	   for (k=Kmx/2;k<Kmx;k++)
	   if (DF[k]<0) {Kr=k;break;}
	   for (k=Kr;k<Kmx;k++) w[clr-1]+=(Tm[k]-Ch[k])/(Ch[Kr]*(Kmx-Kr));
	   done = findnext(&ffblk);
	 }
	 getch();
	 restext();

	 fdat[0]=0;
	 strcpy(fdat,PathToDat());strcat(fdat,"W.dat");
	 cl=fopen(fdat,"wt");
	 if (w[0]!=0)
	 for(k=0;k<clr;k++)
	 { w[k]/=w[clr-1];
	   fprintf(cl,"%e\n",w[k]);
	 }
	 fclose(cl);

  }else
   {gotoxy(20,12);
	printf("there's no files with *.gra* extentions");
	getch();
   }
  delete WL;
  delete Ch;
  delete Tm;
  delete DF;
}

