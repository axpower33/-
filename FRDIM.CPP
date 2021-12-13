#include "sgrbox.h"
#include "fractal.h"
#include "svgacc.h"
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

const KMAX=20;
int   Kmax;
int   ScR;
float Koeff;
float RNG;

float RnGr=0.01;
typedef float carray[KMAX];

float FrDim[Npart/2], Rch[Npart/2];
float Sqd[KMAX][Npart/2];
float Sqdmax[KMAX][Npart/2];
Particle *Pk;
int i,Km;

int gm,gd;
float Rcr,L;
carray FSqd;
carray FSqdmax;

float MinSq(float* X,float* Y)
{    float AvXY,AvSqX,Dx;
	 AvXY=AvSqX=0;
	 for(int k=1;k<Km+1;k++)
	 { AvXY=AvXY+X[k]*Y[k]/Kmax;
	   AvSqX=AvSqX+X[k]*X[k]/Kmax;
	 }
	 return (AvXY/AvSqX);
}

float FunMax(float* fun)
{ float Cm;
  Cm=fun[1];
  for(int k=2;k<Km+1;k++) if (Cm<fun[k]) Cm=fun[k];
  return Cm;
}

float FunMin(float* fun)
{ float Cm;
  Cm=fun[1];
  for(int k=2;k<Km+1;k++) if (Cm>fun[k]) Cm=fun[k];
  return Cm;
}

void Graphic(float *X, float*Y, float FrD,int Cfr)
{float Xmax=FunMax(X);
 float Xmin=FunMin(X);
 float Ymax=FunMax(Y);
 float Ymin=FunMin(Y);
 InitGraph((1-RnGr)*Xmin,(1-RnGr)*Ymin,(1+RnGr)*Xmax,(1+RnGr)*Ymax);
 RangeBox(RNG);
 Box(4);
 int k;
 for (k=1;k<Km;k++) Line(RED,X[k],Y[k],X[k+1],Y[k+1]);
 for (k=1;k<Km+1;k++) FillEllipse(BLACK,X[k],Y[k],4/rangeX,3/rangeY);
 char s1[20];
 gcvt(FrD,5,s1);
 char ss[50]; strcpy(ss,"D=");
 strcat(ss,s1); OutTextXY(BLUE,(1-RnGr)*Xmin+2.5*rng*oldScX,(1+RnGr)*Ymax+0.5*rng*oldScY,ss);
 strcpy(ss,"Fractal Dimension ");
 itoa(Cfr,s1,10);strcat(ss,s1);
 strcat(ss,"th agregate");
 OutTextXY(BLUE,(1-RnGr)*Xmin+1.55*rng*oldScX,(1+RnGr)*Ymax+0.9*rng*oldScY,ss);
 getch();
 restext();
}

void FracDim()
{ MakeArray(N);
  Load();int Nc;int k,j;

  for (i=1;i<s;i++)
  { Rch[i]=0;Nc=0;
    k=Pagregat[i];
    while (k)
    { Pk=Npat(k);
      j=ConPat[k];
      while(j)
      {Pj=Npat(j);
       L=sqrt(pow(Pj->X-Pk->X,2)+pow(Pj->Y-Pk->Y,2)+pow(Pj->Z-Pk->Z,2));
       if (L<Pj->R+Pk->R+Rmax/10)
       { Nc++;
	 Rch[i]+=L;
       }
       j=ConPat[j];
      }
      k=ConPat[k];
    }
    Rch[i]*=Koeff/Nc;
  }

  InitGraph(-Xmax,-Ymax,Xmax,Ymax);
  for (int i=1;i<s;i++)
  { Circle (LIGHTGREEN,CMass[i].X, CMass[i].Y, 1/rangeR);
    j=Pagregat[i];
    while (j)
    { Pj=Npat(j);
      if (Pj->q<0)
      { Circle(LIGHTBLUE,Pj->X, Pj->Y,Pj->R);
	Line(LIGHTBLUE,Pj->X-2/rangeX, Pj->Y,Pj->X+2/rangeX,Pj->Y);
      }
      else
      { Line(LIGHTRED,Pj->X-2/rangeX, Pj->Y,Pj->X+2/rangeX,Pj->Y);
	Line(LIGHTRED,Pj->X, Pj->Y-2/rangeY,Pj->X,Pj->Y+2/rangeY);
	Circle(LIGHTRED,Pj->X, Pj->Y,Pj->R);
      }
      j=ConPat[j];
    }
    for (k=1;k<Kmax+1;k++)
    { Rcr=k*ScR*Rmax;
      Circle(YELLOW,CMass[i].X, CMass[i].Y,Rcr);
      int n=0;
      j=Pagregat[i];
      while (j)
      { Pj=Npat(j);
	L=sqrt(pow(Pj->X-CMass[i].X,2)+pow(Pj->Y-CMass[i].Y,2)+pow(Pj->Z-CMass[i].Z,2));
	if (L<Rcr) n++;
	j=ConPat[j];
      }
      Sqd[k][i]=n;
      Sqdmax[k][i]=Rcr;
    }
    char ss[3];
    itoa(i,ss,10);
    OutTextXY (WHITE,CMass[i].X, CMass[i].Y,ss);
  }
  getch();

  for(i=1;i<s;i++)
  { for(k=1;k<Kmax+1;k++)
    { FSqd[k]=log(Sqd[k][i]);
      FSqdmax[k]=log(Sqdmax[k][i]/Rch[i]);
      Km=Kmax;
      if (FSqd[k]==FSqd[k-1])
      {Km=k-1; k+=Kmax; continue;}
    }
    FrDim[i]=MinSq(FSqdmax,FSqd);
    Graphic(FSqdmax,FSqd,FrDim[i],i);
  }
  DistroyArray();
}






