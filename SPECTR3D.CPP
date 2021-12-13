#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <alloc.h>
#include <math.h>
#include "sgrbox.h"
#include "svgacc.h"
#include "array.h"
#include "fractal.h"

typedef unsigned int uint;
int N;
int nn;
const Kmx=100;
float Xmn;
float Xmx;
float del;
float** As;
float** P;
float** U;
float* W;
float* Xcl;
float* Ycl;
float* Zcl;
float* X;
float* XX;
float* YY;
float* ZZ;
float* Chi;
int countCall;

bool FormArray()
{ bool mem=false;
  printf("\nbefore array %lu",farcoreleft());
  if (!(As=(float**)makeAr(nn,(nn+1)/2+1,sizeof(float)))) mem=true;
  printf("\nafter As=%lu", farcoreleft());

  if (!(P=(float**)makeAr(nn,nn,sizeof(float)))) mem=true;
  printf("\nafter P=%lu", farcoreleft());

  if (!(W=new float[nn])) mem=true;
  printf("\nafter W=%lu",farcoreleft());

  if (!(U=(float**)makeAr(nn,nn,sizeof(float)))) mem=true;
  printf("\nafter U=%lu",farcoreleft());

  if (!(Xcl=new float[N])) mem=true;
  printf("\nafter Xcl=%lu",farcoreleft());

  if (!(Ycl=new float[N])) mem=true;
  printf("\nafter Ycl=%lu",farcoreleft());

  if (!(Zcl=new float[N])) mem=true;
  printf("\nafter Zcl=%lu",farcoreleft());

  if (!(XX=new float[Kmx])) mem=true;
  printf("\nafter XX=%lu",farcoreleft());

  if (!(YY=new float[Kmx])) mem=true;
  printf("\nafter YY=%lu",farcoreleft());

  if (!(ZZ=new float[Kmx])) mem=true;
  printf("\nafter ZZ=%lu",farcoreleft());

  if (!(Chi=new float[Kmx])) mem=true;
  printf("\nafter Chi=%lu",farcoreleft());

  if (!(X=new float[Kmx])) mem=true;
  printf("\nafter X=%lu",farcoreleft());
  if (!mem) return true;
  return false;
}

void DestrAr()
{  delete[] Xcl;
   delete[] Ycl;
   delete[] Zcl;
   delete[] W;
   delAr((void**)U);
   delAr((void**)As);
   delAr((void**)P);
   delete[] XX;
   delete[] YY;
   delete[] ZZ;
   delete[] Chi;
   delete[] X;
}

float funMax(float* fun)
{ float Cm;
  Cm=fun[0];
  for (int k=1;k<Kmx;k++) if (Cm<fun[k]) Cm=fun[k];
  return Cm;
}

float funMin(float* fun)
{ float Cm;
  Cm=fun[0];
  for (int k=1;k<Kmx;k++) if (Cm>fun[k]) Cm=fun[k];
  return Cm;
}

void GraphIm(float* X, float* fun)
{ float RNG=0.17;
  InitGraph(Xmn,funMin(fun),Xmx,funMax(fun));
  RangeBox(RNG);
  Box(4);
  for (int k=1;k<Kmx;k++) Line(RED,X[k-1], fun[k-1],X[k], fun[k]);
  getch();
  restext();
}

void Deigen(int n,int mv)
{ const float range=0.00001;
  uint iq,mm,ll,lq,mq,jq,ind,ij,i,j;
  uint l,m,lm,ilq,imq,im,il,imr,ilr,lmm;
  double anorm,x,y,sinx,sinx2,cosx,cosx2,sincs;
  double thr,anrmx;
  if (mv!=1)
   { iq=-n;
	 for (j=1;j<n+1;j++)
	 { iq+=n;
	   for(uint i=1;i<n+1;i++)
	   if (i==j) P[(iq+i-1)/nn][(iq+i-1)%nn]=1;
	   else P[(iq+i-1)/nn][(iq+i-1)%nn]=0;
	 }
   }
 anorm=0.0;
 for (j=2;j<n+1;j++)
 for (i=1;i<j;i++)
 { ij=i+(j*j-j)/2;
   anorm+=pow(As[(ij-1)/nn][(ij-1)%nn],2);
 }
 if (anorm>0)
 { anorm=sqrt(anorm)*1.414;
   anrmx=anorm*range/n;
// compute threshold:THR
   ind=0;thr=anorm;
// пpовеpка ноpмы матpицы
   do
   { thr/=n;
	 printf("\n %e	 %e",thr,anrmx);

m50: l=1;
m55: m=l+1;
//   compute SIN and COS

m60: mq=(m*m-m)/2;
	 lq=(l*l-l)/2;
	 lm=l+mq;
	 if (fabs(As[(lm-1)/nn][(lm-1)%nn])>=thr)
	 { ind=1;
	   ll=l+lq;
	   mm=m+mq;
	   x=0.5*(As[(ll-1)/nn][(ll-1)%nn]-As[(mm-1)/nn][(mm-1)%nn]);
	   y=-As[(lm-1)/nn][(lm-1)%nn]/sqrt(pow(As[(lm-1)/nn][(lm-1)%nn],2)+x*x);
	   if (x<0) y=-y;
	   sinx=y/sqrt(2.0*(1.0+(sqrt(1.0-y*y))));
	   sinx2=sinx*sinx;
	   cosx=sqrt(1.0-sinx2);
	   cosx2=cosx*cosx;
	   sincs=sinx*cosx;
//     rotate L and M columns
	   ilq=n*(l-1);
	   imq=n*(m-1);
	   for(i=1;i<n+1;i++)
	   { iq=(i*i-i)/2;
		 if(i!=l)
		 { if (i<m) {im=i+mq;goto m95;}
		   else {if(i==m) goto m115; im = m + iq;}
  m95:	   if (i<l) {il=i+lq; goto m110;}
		   il=l+iq;
  m110:    x=As[(il-1)/nn][(il-1)%nn]*cosx-As[(im-1)/nn][(im-1)%nn]*sinx;
		   As[(im-1)/nn][(im-1)%nn]=As[(il-1)/nn][(il-1)%nn]*sinx+As[(im-1)/nn][(im-1)%nn]*cosx;
		   As[(il-1)/nn][(il-1)%nn]=x;
		 }
  m115:  if (mv!=1)
		 { ilr=ilq+i;
		   imr=imq+i;
		   x=P[(ilr-1)/nn][(ilr-1)%nn]*cosx-P[(imr-1)/nn][(imr-1)%nn]*sinx;
		   P[(imr-1)/nn][(imr-1)%nn]=P[(ilr-1)/nn][(ilr-1)%nn]*sinx+P[(imr-1)/nn][(imr-1)%nn]*cosx;
		   P[(ilr-1)/nn][(ilr-1)%nn]=x;
		 }
	   }
	   x=2.0*As[(lm-1)/nn][(lm-1)%nn]*sincs;
	   y=As[(ll-1)/nn][(ll-1)%nn]*cosx2+As[(mm-1)/nn][(mm-1)%nn]*sinx2-x;
	   x=As[(ll-1)/nn][(ll-1)%nn]*sinx2+As[(mm-1)/nn][(mm-1)%nn]*cosx2+x;
	   As[(lm-1)/nn][(lm-1)%nn]=(As[(ll-1)/nn][(ll-1)%nn]-As[(mm-1)/nn][(mm-1)%nn])*sincs+As[(lm-1)/nn][(lm-1)%nn]*(cosx2-sinx2);
	   As[(ll-1)/nn][(ll-1)%nn]=y;
	   As[(mm-1)/nn][(mm-1)%nn]=x;
	 }
//   test for completion
	 if (m!=n){m++;goto m60;}
	 if (l!=(n-1)){l++;goto m55;}
	 if (ind==1){ind=0;goto m50;}
   } while(thr>anrmx);
 }
 for(i=1;i<nn+1;i++)
 { int k=(i*(i+1))/2;
   printf("\n i=%d   eig=%e",i,As[(k-1)/nn][(k-1)%nn]);
   W[i-1]=As[(k-1)/nn][(k-1)%nn];
 }
 m=0;l=1;
 for (i=1;i<nn*nn+1;i++)
 { m++;
   if (m==nn+1)
   { m=1;l++;
   }
   U[m-1][l-1]=P[(i-1)/nn][(i-1)%nn];
 }
 char fdat[128];
 fdat[0]=0;
 strcpy(fdat,PathToDat());strcat(fdat,"EVAL.DAT");
 FILE *Eval=fopen(fdat,"wt");
 for(i=0;i<nn;i++) fprintf(Eval,"%e\n",W[i]);
 fclose(Eval);

/* fdat[0]=0;
 strcpy(fdat,PathToDat());strcat(fdat,"EVEC.DAT");
 FILE *Evec=fopen(fdat,"wt");
 for (i=0;i<nn;i++)
 { for (j=0;j<nn;j++) fprintf(Evec,"%e	",U[i][j]);
   fprintf(Evec,"\xD");
 }
 fclose(Evec);*/
}

bool Spectr3d()
{uint i,j;
 float z,h;
 double Ux,Uy,Uz;
 float Uux,Uuy,Uuz;
 if (!FormArray()) return false;

 char fdat[128];
 fdat[0]=0;
 strcpy(fdat,PathToDat());strcat(fdat,"frsp.dat");
 FILE *cl=fopen(fdat,"rt");
 for(i=0;i<N;i++)
 fscanf(cl,"%e %e %e",&Xcl[i], &Ycl[i], &Zcl[i]);
 fclose(cl);

 for (i=0;i<N;i++)
 for (j=0;j<N;j++)
 if (i!=j)
 { float rx=Xcl[i]-Xcl[j];
   float ry=Ycl[i]-Ycl[j];
   float rz=Zcl[i]-Zcl[j];
   float r2=rx*rx+ry*ry+rz*rz;
   float r12=sqrt(r2);
   float r5=1.0/(r2*r2*r12);
   U[3*i][3*j]=(r2-3*rx*rx)*r5;
   U[3*i][3*j+1]=-3*rx*ry*r5;
   U[3*i][3*j+2]=-3*rx*rz*r5;
   U[3*i+1][3*j]=U[3*i][3*j+1];
   U[3*i+1][3*j+1]=(r2-3*ry*ry)*r5;
   U[3*i+1][3*j+2]=-3*ry*rz*r5;
   U[3*i+2][3*j]=U[3*i][3*j+2];
   U[3*i+2][3*j+1]=U[3*i+1][3*j+2];
   U[3*i+2][3*j+2]=(r2-3*rz*rz)*r5;
 } else
   { U[3*i][3*j]=0;
	 U[3*i][3*j+1]=0;
	 U[3*i][3*j+2]=0;
	 U[3*i+1][3*j]=0;
	 U[3*i+1][3*j+1]=0;
	 U[3*i+1][3*j+2]=0;
	 U[3*i+2][3*j]=0;
	 U[3*i+2][3*j+1]=0;
	 U[3*i+2][3*j+2]=0;
   }
   int kl=0;
   for (i=0;i<nn;i++)
   for (j=0;j<nn;j++)
   if (i>=j)
   { kl++;
    As[(kl-1)/nn][(kl-1)%nn]=U[i][j];
//   printf("\n<%u|W|%u>=%e",(i+1)/2,(j+1)/2,U[i][j]);
   }
   Deigen(nn,0);
   int k;
   for (k=0;k<Kmx;k++)
   { XX[k]=0;
     YY[k]=0;
     ZZ[k]=0;
   }
//   clrscr();

   for (k=0;k<Kmx;k++)
   { X[k]=Xmn+k*(Xmx-Xmn)/Kmx;
     printf("\nK=%u",k);
     for(int n=0;n<nn;n++)
     { z=-X[k]+W[n];
       for(i=0;i<N;i++)
       { Uux=U[3*i][n];
	 Uuy=U[3*i+1][n];
	 Uuz=U[3*i+2][n];
	 h=z*z+del*del;
	 for(j=0;j<N;j++)
	 { Ux=Uux*U[3*j][n]/h;
	   Uy=Uuy*U[3*j+1][n]/h;
	   Uz=Uuz*U[3*j+2][n]/h;
	   XX[k]=XX[k]+del*Ux;
	   YY[k]=YY[k]+del*Uy;
	   ZZ[k]=ZZ[k]+del*Uz;
	 }
       }
     }
   }

   for (k=0;k<Kmx;k++)
   {
    XX[k]=XX[k]/(3*N);
    YY[k]=YY[k]/(3*N);
    ZZ[k]=ZZ[k]/(3*N);
    Chi[k]=(XX[k]+YY[k]+ZZ[k]);
   }
   GraphIm(X,Chi);

   fdat[128];
   char nmSp[20];
   char* cnCl;
   fdat[0]=0;
   nmSp[0]=0;
   strcpy(fdat,PathToDat());
   strcpy(nmSp,"spectr");
   itoa(countCall,cnCl,10);
   strcat(nmSp,cnCl);
   strcat(nmSp,".gra");
   strcat(fdat,nmSp);
   FILE *Spectr=fopen(fdat,"wt");
   for(k=0;k<Kmx;k++) fprintf(Spectr,"%e \n",Chi[k]);
   fclose(Spectr);
   DestrAr();
   return true;
}
