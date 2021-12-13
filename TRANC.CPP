#include<stdio.h>
#include<math.h>

float Lmin=1e-7;
float Lmax=1e-6;
float L0=4.2e-7;
const kmax=100;
double W[kmax],L[kmax],X[kmax],Wn[kmax];
float c=3e8;
float R=12e-9;
float h=1.05e-34;
float W0=0;
void main()
{
  int k;
  for(k=0;k<kmax;k++)
  {  L[k]=Lmin+(Lmax-Lmin)*k/kmax;
     W0=2*M_PI*c/L0;
     W[k]=W0-2*M_PI*c*(L[k]-L0)/(L[k]*L[k]);
     Wn[k]=2*M_PI*c/L[k];
     X[k]=(W[k]-W0)/(W0*8*R*R*R);
  }
}
