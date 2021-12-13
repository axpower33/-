#include <stdlib.h>
#include <fcntl.h>
#include <conio.h>
#include <io.h>
#include <sys\stat.h>
#include <math.h>
#include "sgrbox.h"
#include "fractal.h"
#include "svgacc.h"
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <dir.h>
#include <alloc.h>
//#include "debug.h"

#define Rmx 35
bool WorkShow = false;
float DX;
float DY;
int pg=0;

struct v3
{ float x, y, z;
};

//-----------Изменяемые програмные параметры------------//
bool needCharge;
bool needCulon;
bool needVDVaalse;
bool needLoad;
bool needSave;
bool needRand = true;

//---------- Физические величины -----------//
float dt;                    //c       Шаг по времени
float Rmax;                  //cм      Максимальный радиус у частицы
float Rmin;                  //cм      Минимальный радиус у частицы
float Rmid;                  //cм      Среднее расстояние между частицами
float Tmshft;            //с       Время свободного пробега частиц
float DensAg;            //г/cм^3  Плотность серебра
int Tk;                      //K       Температура
int MaxQ;                    //        Максимальный заряд
//------------Электрические константы-----------------//
float kk=1.38E-23;           //Дж/K    Постоянная Больцмана
float E0=8.854E-14;          //Ф/cм    Электрическая постоянная
int E=100;                       //        Диэлектрическая проницаемость
float C_SI=1/(4*M_PI*E0*E);//      Коэффициент в СИ
float eQulon=1.6E-19;      //Кл   Заряд электрона

struct IPoint
       {int X;
        int Y;
        int Z;}A,B;

Particle* PNp;
Agregat dF[Npart/2] ;
int Pagregat[Npart/2];
Agregat CMass[Npart/2];
int ConPat[Npart+1];
float TmPat[Npart+1];
int s; float t;
unsigned char VDn;
float OldRX; float OldRY;
Particle* FirstPat;
Particle* LastPat;
Particle* TempP;
Particle* Pi;
Particle* Pj;
Particle** Mp=NULL;
float Xmax, Ymax, Zmax;
unsigned short LdSv;
unsigned short PatInt;
char path[128];
char ch;

RasterBlock* pb[Rmx], *pr[Rmx],*pm[Rmx];

void interrupt (*KeyInt)(...);

void MakeArray(int N)
{   FirstPat=new Particle;
    FirstPat->pred=NULL;
    FirstPat->N=1;
    LastPat=FirstPat;
    for(int i=2;i!=N+1;i++)
    {  LastPat->next=TempP=new Particle;
       TempP->pred=LastPat;
       TempP->next=NULL;
       TempP->N=i;
       LastPat=TempP;
    }
    Mp=new Particle*[N];
}


void DistroyArray()
{ for(TempP=FirstPat->next;TempP->next!=NULL;TempP=TempP->next)
  delete TempP->pred;
  delete TempP;
  delete[] Mp;
}

Particle* Npat(int Np)
{ TempP=FirstPat;
  for(int i=1;i<Np;i++) TempP=TempP->next;
  return (TempP);
}

void SizePatDistr()
{ const  Smx=25;
  int k;
  float sizep[Smx];
  int Kol[Smx];
  char fdat[128];
  fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"sizepat.dat");
  _fmode=O_TEXT;
  FILE *cl=fopen(fdat,"rt");
  if (cl!=NULL)
  { for (k=0;k<Smx;k++)
    { if (!feof(cl))
      { fscanf(cl,"%e %d",&sizep[k],&Kol[k]);
        sizep[k]*=1e-7;
      } else break;
    }
    fclose(cl);
    Pi=FirstPat;
    for(k=0;k<Smx;k++)
    { while(Kol[k])
      { Pi->R=sizep[k];
        Pi=Pi->next;
        Kol[k]--;
      }
    }
  }
  else
  for(Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  if (Pi->N<=N*0.2) Pi->R=Rmin;
  else if (Pi->N<=N*0.8) Pi->R=(Rmin+Rmax)/2;
       else Pi->R=Rmax;
}

void SpontDstr()
{ float Vel,alpha,theta;
  for(Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  { Vel=1.41*sqrt(2*kk*Tk/Pi->mass)*float(rand())/RAND_MAX;
    alpha=2*M_PI* float(rand())/RAND_MAX;
    theta=2*M_PI* float(rand())/RAND_MAX;
    Pi->Vx=Vel*cos(theta)*sin(alpha);
    Pi->Vy=Vel*cos(theta)*cos(alpha);
    Pi->Vz=Vel*sin(theta);
  }
}

void zeroDstr()
{for(Pi=FirstPat;Pi!=NULL;Pi=Pi->next) Pi->Vx=Pi->Vy=Pi->Vz=0;}

void MxwDstr()
{ const Vmin=0;
  float Vmax=0.25;
  int kmax=N/3;
  int i,j;
  double F;
  float dN,V,Vel,dv,alpha,theta;
  int rdn ;

  j=0; i=1;
  dv=(Vmax-Vmin)/kmax;

  Pj=FirstPat;

  for(Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  {  V=Vmin+dv*i;
     F=sqrt(Pi->mass/(2*M_PI*kk*Tk));
     F=pow(F,3);
     F=F*exp(-(Pi->mass*pow(V,2))/(2*kk*Tk))*4*M_PI*pow(V,2);
     dN=N*F*dv;
     rdn=(int)(dN+0.5);
     i++;
     if ((j+rdn)<N)
     while (rdn)
     { j++;
       Vel=V;
       alpha=2*M_PI* float(rand())/RAND_MAX;
       theta=2*M_PI* float(rand())/RAND_MAX;
       Pj->Vx=Vel*cos(theta)*sin(alpha);
       Pj->Vy=Vel*cos(theta)*cos(alpha);
       Pj->Vz=Vel*sin(theta);
       rdn--;
       Pj=Pj->next;
     }
  }
  int cnt=N-j;

  while (cnt--)
  { Vel=sqrt((8*kk*Tk)/(M_PI*Pj->mass));
    alpha=2*M_PI*float(rand())/RAND_MAX;
    theta=2*M_PI*float(rand())/RAND_MAX;
    Pj->Vx=Vel*cos(theta)*sin(alpha);
    Pj->Vy=Vel*cos(theta)*cos(alpha);
    Pj->Vz=Vel*sin(theta);
    Pj=Pj->next;
  }
}

void InitParticle()
{   int sign=0;
    float sq,alpha;

    SizePatDistr();
    for(Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
    { sign=1-sign;
m1: Pi->X=-Xmax+GetScX*float(rand())/RAND_MAX;
    Pi->Y=-Ymax+GetScY*float(rand())/RAND_MAX;
    Pi->Z=-Xmax+GetScX*float(rand())/RAND_MAX;

    for(Pj=FirstPat;Pj!=Pi;Pj=Pj->next)
    { float dist_ij=pow(Pi->X-Pj->X,2)+pow(Pi->Y-Pj->Y,2)+pow(Pi->Z-Pj->Z,2);
      if (dist_ij<=pow(Pi->R+Pj->R,2)) goto m1;
    }
    if (needCharge) if (sign) Pi->q=MaxQ*eQulon;
    else Pi->q=-MaxQ*eQulon;
    else Pi->q=0;

    Pi->mass=4/3*M_PI*pow(Pi->R,3)/DensAg;
    //TmPat[Pi->N]=Tmshft*float(rand())/RAND_MAX;

    Pi->agr=0;
    Pi->stop=false;
    Pi->VanX=0;
    Pi->VanY=0;
    Pi->VanZ=0;
    Pi->Fx=0;
    Pi->Fy=0;
    Pi->Fz=0;
   }
   switch (VDn)
   {
    case 0: MxwDstr();break;
    case 1: SpontDstr();break;
    case 2: zeroDstr();break;
    default:break;
   }
}

void InitAgr()
{
 int c;
 for(c=0;c!=N/2;c++) Pagregat[c]=0;
 for(c=0;c!=N+1;c++) ConPat[c]=0;
}

bool IsPatInAgr (Particle* Pi,Particle* Pj)
{
  return  (bool) ((Pi->agr!=0)&&(Pi->agr==Pj->agr));
}

void VanDerVaalce()
{ float  dX, dY, dZ;
  double Temp;
  double h=6.626e-34;
  double c=3e10;
  double Lm=4.1e-5;

  for (Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  { Pi->VanX=0;
    Pi->VanY=0;
    Pi->VanZ=0;
  }
  double Cv=-0.75*(h*c/Lm);
  for (Pi=FirstPat; Pi!=NULL;Pi=Pi->next)
  { for (Pj=Pi->next;Pj!=NULL;Pj=Pj->next)
    { if (IsPatInAgr(Pi,Pj)) continue;
      dX=Pi->X-Pj->X;
      dY=Pi->Y-Pj->Y;
      dZ=Pi->Z-Pj->Z;
      Temp=pow(Pi->R,6)/pow(dX*dX+dY*dY+dZ*dZ,3.5);
//    Temp=-fabs(Pi->q*Pj->q*pow(2*(Pi->R+Pj->R),4))/pow(dX*dX+dY*dY+dZ*dZ,3.5);

      Pj->VanX-=Temp*dX;
      Pj->VanY-=Temp*dY;
      Pj->VanZ-=Temp*dZ;
      Pi->VanX+=Temp*dX;
      Pi->VanY+=Temp*dY;
      Pi->VanZ+=Temp*dZ;
    }
//  float U=Pi->VanX*dX+Pi->VanY*dY+Pi->VanZ*dZ;
//  float Ek=Pi->mass*(pow(Pi->Vx,2)+pow(Pi->Vy,2)+pow(Pi->Vz,2))/2;
  }
}

void CulonForce()
{ int Nagr;
  float Temp, dX, dY, dZ;
  for (Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  { Pi->Fx=0;
    Pi->Fy=0;
    Pi->Fz=0;
  }
  for (Pi=FirstPat; Pi!=NULL;Pi=Pi->next)
  { for (Pj=Pi->next;Pj!=NULL;Pj=Pj->next)
    { if (IsPatInAgr(Pi,Pj)) continue;
      dX=Pi->X-Pj->X;
      dY=Pi->Y-Pj->Y;
      dZ=Pi->Z-Pj->Z;
      Temp=Pi->q*Pj->q/pow(dX*dX+dY*dY+dZ*dZ,1.5);
      Pj->Fx-=Temp*dX;
      Pj->Fy-=Temp*dY;
      Pj->Fz-=Temp*dZ;
      Pi->Fx+=Temp*dX;
      Pi->Fy+=Temp*dY;
      Pi->Fz+=Temp*dZ;
    }
    Pi->Fx*=C_SI;
    Pi->Fy*=C_SI;
    Pi->Fz*=C_SI;
//  float U=Pi->Fx*dX+Pi->Fy*dY+Pi->Fz*dZ;
//  float Ek=Pi->mass*(pow(Pi->Vx,2)+pow(Pi->Vy,2)+pow(Pi->Vz,2))/2;
  }
}

void BallinHeap()
{ int i;int x,y,r;
  _fmode=O_BINARY;
  int f1=open("pr.dat",S_IREAD);
  int f2=open("pb.dat",S_IREAD);
  int f3=open("pm.dat",S_IREAD);
  for(r=0;r<Rmx;r++)
  { pr[r]=NULL;
    pb[r]=NULL;
    pm[r]=NULL;
  }
  if (FirstPat->q!=0)
  {  for(r=0;r<Rmx;r++)
     { i=(2*r+1)*(2*r+1)+4;
       pr[r]=(RasterBlock*)malloc(i);
       read(f1,pr[r],i);
     }
     for(r=0;r<Rmx;r++)
     { i=(2*r+1)*(2*r+1)+4;
       pb[r]=(RasterBlock*)malloc(i);
       read(f2,pb[r],i);
     }
  }else
   for(r=0;r<Rmx;r++)
   { i=(2*r+1)*(2*r+1)+4;
     pm[r]=(RasterBlock*)malloc(i);
     read(f3,pm[r],i);
   }
   close(f1);
   close(f2);
   close(f3);
}

void BalloffHeap()
{
 for(int r=0;r<Rmx;r++)
 { if (pm[r]!=NULL) free(pm[r]);
   if (pb[r]!=NULL) free(pb[r]);
   if (pr[r]!=NULL) free(pr[r]);
 }
}

#define Pprt(a) (*(Particle**)a)
int sortOfZ (const void *a, const void *b)
{
  if (Pprt(a)->Z>Pprt(b)->Z) return  1; else
  if (Pprt(a)->Z<Pprt(b)->Z) return -1; else
  return  0;
}

void HiResPict()
{ int i;int x,y,r;
  for (i=0, Pi=FirstPat;(Mp[i++]=Pi, Pi=Pi->next); );
  qsort(Mp, N, sizeof(Particle*), &sortOfZ);
  float Dzx,Dzy;

  for(i=0;i<N;i++)
  {
   if (Mp[i]->agr!=0)
   { Dzx=Mp[i]->Z*(Mp[i]->X-CMass[Mp[i]->agr].X)/Zmax;
     Dzy=Mp[i]->Z*(Mp[i]->Y-CMass[Mp[i]->agr].Y)/Zmax;
   } else Dzx=Dzy=0;

   x=maxx/2+(Mp[i]->X+Dzx)*rangeX;
   y=maxy/2-(Mp[i]->Y+Dzy)*rangeY;
   r=Mp[i]->R*(1+Mp[i]->Z/Zmax)*sqrt(rangeX*rangeX+rangeY*rangeY)*0.83;
   if (r<0) r=0; if (r>=Rmx) r=Rmx-1;

   if (Mp[i]->q<0) spriteput(SET,0,x-r,y-r,pb[r]);
   else if (Mp[i]->q==0) spriteput(SET,0,x-r,y-r,pm[r]);
   else spriteput(SET,0,x-r,y-r,pr[r]);
  }
  Rectangle(GREEN,GetminX+DX,GetminY+DY,GetmaxX+DX,GetmaxY+DY);
  pagedisplay (0,0,pg);
  pg=1-pg;
  pageactive (pg);
  fillpage(0);
}

void ShowPicture()
{ char st[100];
  float Dzx,Dzy;
  if (WorkShow) return;
  WorkShow=true;

  for(Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  {if (Pi->agr!=0)
   { Dzx=Pi->Z*(Pi->X-CMass[Pi->agr].X)/Zmax;
     Dzy=Pi->Z*(Pi->Y-CMass[Pi->agr].Y)/Zmax;
   } else Dzx=Dzy=0;

   if (Pi->q==0) Circle(MAGENTA,Pi->X+Dzx+DX, Pi->Y+Dzy+DY, Pi->R+Pi->Z*Pi->R/Zmax);
   else
   if (Pi->q<0)
   { Line(BLUE,Pi->X+Dzx+DX-2.0/rangeX, Pi->Y+Dzy+DY, Pi->X+Dzx+2.0/rangeX+DX, Pi->Y+Dzy+DY);
     Circle(BLUE,Pi->X+Dzx+DX, Pi->Y+Dzy+DY, Pi->R+Pi->Z*Pi->R/Zmax);
   }
   else
   { Line(RED,Pi->X+Dzx+DX-2.0/rangeX, Pi->Y+Dzy+DY, Pi->X+Dzx+2.0/rangeX+DX, Pi->Y+Dzy+DY);
     Line(RED,Pi->X+Dzx+DX, Pi->Y-2.0/rangeY+Dzy+DY, Pi->X+Dzx+DX, Pi->Y+Dzy+2.0/rangeY+DY);
     Circle(RED,Pi->X+Dzx+DX, Pi->Y+Dzy+DY, Pi->R+Pi->Z*Pi->R/Zmax);
   }

/* Setcolor (WHITE);
   itoa (Pi->N,st,10);
   OutTextXY(Pi->X+DX, Pi->Y+DY, st);
*/
  if (!Pi->stop) Line(LIGHTGRAY,Pi->X+DX, Pi->Y+DY,Pi->X+DX+Pi->Fx*1e10, Pi->Y+DY+Pi->Fy*1e10);
  }
  for (int i=1;i<s;i++)
  {
   Circle (GREEN,CMass[i].X+DX, CMass[i].Y+DY, 1.0/rangeR);
   Line(LIGHTGRAY,CMass[i].X+DX,CMass[i].Y+DY,CMass[i].X+DX+dF[i].X*1e10, CMass[i].Y+DY+dF[i].Y*1e10);
  }

  Rectangle(GREEN,GetminX+DX,GetminY+DY,GetmaxX+DX,GetmaxY+DY);
  drwline(SET,RED,maxx/2-2,maxy/2,maxx/2+2,maxy/2);
  drwline(SET,RED,maxx/2,maxy/2-2,maxx/2,maxy/2+2);
  drwbox(SET,RED,maxx/2-4,maxy/2+4,maxx/2+4,maxy/2-4);

  pagedisplay (0,0,pg);
  pg=1-pg;
  pageactive (pg);
  fillpage(0);
  WorkShow=false;
}

bool int9Work = false;

void far finishInt9h()
{
 asm{
         in al,61h
         mov ah,al
         or al,80h
         out 61h,al
         xchg ah,al
         out 61h,al
         mov al,20h
         out 20h,al
        }
}

void interrupt ScPict(...)
{ unsigned char  scanCode, keyCode;
  unsigned char  fpuState[108];

  scanCode = inport(0x60);
  keyCode = (scanCode & 0x7F);

  if (int9Work) { finishInt9h(); return; }
  int9Work = true;

  switch (keyCode)
  { case 78: case 74: case 75: case 77: case 80:
    case 73: case 79: case 81: case 71: case 76:
    case 72: case 1:
    if ((scanCode & 0x80))
    { finishInt9h();
      int9Work = false;
      return;
    } break;
    default:
    { KeyInt();
      int9Work = false;
      return;
    }
  }

  finishInt9h();

  asm{
    sti
    push ds
    mov ax,ss
    mov ds,ax
    lea di,fpuState
    fsave ds:[di]
    pop ds
   }

   switch (keyCode)
   { case 78:
     { DX-=maxx/(2*rangeX);
       DY+=maxy/(2*rangeY);
       rangeX*=1.05;
       rangeY*=1.05;
       DX+=maxx/(2*rangeX);
       DY-=maxy/(2*rangeY);
     } break;
     case 74:
     { DX-=maxx/(2*rangeX);
       DY+=maxy/(2*rangeY);
       rangeX*=(1.0/1.05);
       rangeY*=(1.0/1.05);
       DX+=maxx/(2*rangeX);
       DY-=maxy/(2*rangeY);
     } break;
     case 76:
     { DX=0; DY=0;
       rangeX=OldRX; rangeY=OldRY;
     } break;
     case 75: DX+=GetScX/150;break;
     case 77: DX-=GetScX/150;break;
     case 80: DY+=GetScY/150;break;
     case 72: DY-=GetScY/150;break;
     case 71: DX+=GetScX/5;break;
     case 79: DX-=GetScX/5;break;
     case 81: DY+=GetScY/5;break;
     case 73: DY-=GetScY/5;break;
     case 1: ch='q';break;
   }

//   ShowPicture();
   asm{
   push ds
   mov ax,ss
   mov ds,ax
   lea di,fpuState
   frstor ds:[di]
   pop ds
   }
   int9Work = false;
}

void RandWalk(float& TmP, float& Vlx,float& Vly,float& Vlz)
{ int sign;
  if (TmP<t)
  { sign=(2*random(2)-1);
    Vlx=Vlx*sign;
    sign=(2*random(2)-1);
    Vly=Vly*sign;
    sign=(2*random(2)-1);
    Vlz=Vlz*sign;
    TmP+=Tmshft*float(rand())/RAND_MAX;
  }
}

void MovePart()
{
 int i,j;
 char st[10];
 float dVx,dVy,dVz;

 for (Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
 if (Pi->stop==false)
 { WorkShow=true;
   dVx=dt*Pi->Fx/Pi->mass;
   dVy=dt*Pi->Fy/Pi->mass;
   dVz=dt*Pi->Fz/Pi->mass;
   Pi->Vx+=dVx;
   Pi->Vy+=dVy;
   Pi->Vz+=dVz;
// RandWalk(TmPat[Pi->N],Pi->Vx,Pi->Vy,Pi->Vz);
   if ((Pi->X>GetmaxX)||(Pi->X<GetminX)) Pi->Vx=-Pi->Vx;
   if ((Pi->Y>GetmaxY)||(Pi->Y<GetminY)) Pi->Vy=-Pi->Vy;
   if ((Pi->Z>GetmaxX)||(Pi->Z<GetminX)) Pi->Vz=-Pi->Vz;
   Pi->X+=dt*Pi->Vx;
   Pi->Y+=dt*Pi->Vy;
   Pi->Z+=dt*Pi->Vz;
   WorkShow = false;
 }
}

void AngleSpeed (int kk, RPoint &W)
{
// Полный момент сил, действующий на агрегат со стороны остальных частиц
   float Nx=0; float Ny=0; float Nz=0;

   int i=Pagregat[kk];
   while (i)
   { Nx+=(Pi->Y-CMass[kk].Y)*Pi->Fz-(Pi->Z-CMass[kk].Z)*Pi->Fy;
     Ny+=(Pi->Z-CMass[kk].Z)*Pi->Fx-(Pi->X-CMass[kk].X)*Pi->Fz;
     Nz+=(Pi->X-CMass[kk].X)*Pi->Fy-(Pi->Y-CMass[kk].Y)*Pi->Fx;
     i=ConPat[i];
   }
   CMass[kk].Mx+=dt*Nx;
   CMass[kk].My+=dt*Ny;
   CMass[kk].Mz+=dt*Nz;

   W.X=CMass[kk].Mx/CMass[kk].Jx;
   W.Y=CMass[kk].My/CMass[kk].Jy;
   W.Z=CMass[kk].Mz/CMass[kk].Jz;
}

// Вычисляет центр масс, полный момент, момент инерции агрегата Nagr
void ChangeCMass(int Nagr)
{
  int Np=Pagregat[Nagr];
  float Mass=0;
  CMass[Nagr].X=0;
  CMass[Nagr].Y=0;
  CMass[Nagr].Z=0;

  CMass[Nagr].Vx=0;
  CMass[Nagr].Vy=0;
  CMass[Nagr].Vz=0;
// Вычисление центра масс агрегата Nagr

 while (Np)
 {
   PNp=Npat(Np);
   Mass+=PNp->mass;
   CMass[Nagr].X+=PNp->mass*PNp->X;
   CMass[Nagr].Y+=PNp->mass*PNp->Y;
   CMass[Nagr].Z+=PNp->mass*PNp->Z;

   CMass[Nagr].Vx+=PNp->mass*PNp->Vx;
   CMass[Nagr].Vy+=PNp->mass*PNp->Vy;
   CMass[Nagr].Vz+=PNp->mass*PNp->Vz;

   Np=ConPat[Np];
 }

   CMass[Nagr].X/=Mass;
   CMass[Nagr].Y/=Mass;
   CMass[Nagr].Z/=Mass;

   CMass[Nagr].Vx/=Mass;
   CMass[Nagr].Vy/=Mass;
   CMass[Nagr].Vz/=Mass;

// Вычисление полного момента M и момента инерции J агрегата Nagr

  Np=Pagregat[Nagr];

  CMass[Nagr].Jx=0;
  CMass[Nagr].Mx=CMass[Nagr].My=CMass[Nagr].Mz=0;

 while (Np)
 { PNp=Npat(Np);
   CMass[Nagr].Jx+=PNp->mass*((pow(PNp->X-CMass[Nagr].X,2)+0.4*pow(PNp->R,2)));
   CMass[Nagr].Jy+=PNp->mass*((pow(PNp->Y-CMass[Nagr].Y,2)+0.4*pow(PNp->R,2)));
   CMass[Nagr].Jz+=PNp->mass*((pow(PNp->Z-CMass[Nagr].Z,2)+0.4*pow(PNp->R,2)));

   CMass[Nagr].Mx+=PNp->mass*((PNp->Y-CMass[Nagr].Y)*(PNp->Vz-CMass[Nagr].Vz)-(PNp->Z-CMass[Nagr].Z)*(PNp->Vy-CMass[Nagr].Vy));
   CMass[Nagr].My+=PNp->mass*((PNp->Z-CMass[Nagr].Z)*(PNp->Vx-CMass[Nagr].Vx)-(PNp->X-CMass[Nagr].X)*(PNp->Vz-CMass[Nagr].Vz));
   CMass[Nagr].Mz+=PNp->mass*((PNp->X-CMass[Nagr].X)*(PNp->Vy-CMass[Nagr].Vy)-(PNp->Y-CMass[Nagr].Y)*(PNp->Vx-CMass[Nagr].Vx));

   Np=ConPat[Np];
 }
}

// Процедуры для работы с агрегатами
float GetMass (int i)
{ float ret=0;
  if (Pi->stop)
  { i=Pagregat[Pi->agr];
    do
    { ret+=Pi->mass;
      i=ConPat [i];
    } while(i!=0);
  } else ret = Pi->mass;
  return ret;
}

// Установка линейных скоростей у частиц в агрегате содержащих i
void SetAgrSpeed(Particle* Pi)
{ RPoint W;
  if (Pi->stop)
  {
    //Вычисление линейной скорости частиц в агрегате }
    int i=Pagregat[Pi->agr];
    AngleSpeed(Pi->agr,W);
    do
    { Pi->Vx=CMass[Pi->agr].Vx + W.Y*(Pi->Z-CMass[Pi->agr].Z)-W.Z*(Pi->Y-CMass[Pi->agr].Y);
      Pi->Vy=CMass[Pi->agr].Vy + W.Z*(Pi->X-CMass[Pi->agr].X)-W.X*(Pi->Z-CMass[Pi->agr].Z);
      Pi->Vz=CMass[Pi->agr].Vz + W.X*(Pi->Y-CMass[Pi->agr].Y)-W.Y*(Pi->X-CMass[Pi->agr].X);
      i = ConPat[i];
    }while(i!=0);
  }
}

//  Добавляет j частицу к агрегату начинающемуся с i частицы
void AddPattoAgr (Particle* Pi,Particle* Pj)
{
  Pj->agr=Pi->agr;
  int i=Pi->N; int j=Pj->N;
  while (ConPat[i]) i=ConPat[i];

  ConPat[i]=j;
  if (ConPat[j]!=0)
  {
   j=ConPat[j];
   while (j)
   {
     Pj=Npat(j);
     Pj->agr=Pi->agr;
     j=ConPat[j];
   }
  }
}

//      Рaздвигает две частицы
void PushAway (Particle*Pi,Particle* Pj )
{ float dX, dY, dZ, R;

  R=Pi->R+Pj->R;
  dX=Pj->X-Pi->X;
  dY=Pj->Y-Pi->Y;
  dZ=Pj->Z-Pi->Z;

  float dist_ij=sqrt(dX*dX+dY*dY+dZ*dZ);

  dX*=(R-dist_ij)/dist_ij;
  dY*=(R-dist_ij)/dist_ij;
  dZ*=(R-dist_ij)/dist_ij;

  if (Pj->agr!=0)
  {
   CMass[Pj->agr].X+=dX;
   CMass[Pj->agr].Y+=dY;
   CMass[Pj->agr].Z+=dZ;
   int j=Pagregat[Pj->agr];
   while (j)
   {
     Pj=Npat(j);
     Pj->X+=dX;
     Pj->Y+=dY;
     Pj->Z+=dZ;

     j=ConPat[j];
   }
  } else
    {
      Pj->X+=dX;
      Pj->Y+=dY;
      Pj->Z+=dZ;
    }
}

void UnitPaticle (Particle* Pi, Particle* Pj)
{
 int l, kk, m;
 int i=Pi->N;
 int j=Pj->N;

 Particle* Pm; Particle* Pl;

 if (IsPatInAgr(Pi,Pj)) return;


 PushAway (Pi,Pj);

 SetAgrSpeed(Pi);
 SetAgrSpeed(Pj);

 if (Pi->stop==false)
 {
   if (Pj->stop==false)
   {
    Pagregat[s]=Pi->N;
    Pi->agr=s;
    Pj->agr=s;
    s++;
    ConPat[i]=j;
   } else AddPattoAgr(Pj,Pi);

 } else
   if (Pj->stop==false) AddPattoAgr(Pi,Pj);
   else
   { kk=Pi->agr;
     l=Pagregat[kk];
     s--;
     Pagregat[kk]=Pagregat[s];
     Pagregat[s]=0;
     CMass[kk]=CMass[s];
     CMass[s].X=CMass[s].Y=CMass[s].Z=
     CMass[s].Vx=CMass[s].Vy=CMass[s].Vz=
     CMass[s].Mx=CMass[s].My=CMass[s].Mz=
     CMass[s].Jx=CMass[s].Jy=CMass[s].Jz=0;
     m=Pagregat[kk];
     while (m)
     {
      Pm=Npat(m);
      Pm->agr=kk;
      m=ConPat[m];
     }
     Pl=Npat(l);
     AddPattoAgr(Pj,Pl);
   }

// Пометить i и j как принадлежащие к агрегатам
   Pi->stop=true;
   Pj->stop=true;
   ChangeCMass (Pj->agr);
}

void AgrForces()
{ int i,kk;
  float mass, dVx, dVy, dVz, dX, dY, dZ, M1;

  for(kk=1;kk<s;kk++)
  { i=Pagregat[kk];
    dF[kk].X=dF[kk].Y=dF[kk].Z=mass=0;

    while(i)
    { Pi=Npat(i);
      dF[kk].X+=Pi->Fx;
      dF[kk].Y+=Pi->Fy;
      dF[kk].Z+=Pi->Fz;
      mass+=Pi->mass;
      i=ConPat[i];
    }
    dVx=dt*dF[kk].X/mass;
    dVy=dt*dF[kk].Y/mass;
    dVz=dt*dF[kk].Z/mass;
    if ((CMass[kk].X>GetmaxX)||(CMass[kk].X<GetminX)) CMass[kk].Vx=-CMass[kk].Vx;
    if ((CMass[kk].Y>GetmaxY)||(CMass[kk].Y<GetminY)) CMass[kk].Vy=-CMass[kk].Vy;
    if ((CMass[kk].Z>GetmaxX)||(CMass[kk].Z<GetminX)) CMass[kk].Vz=-CMass[kk].Vz;
    CMass[kk].Vx+=dVx;
    CMass[kk].Vy+=dVy;
    CMass[kk].Vz+=dVz;
    CMass[kk].X+=dt*CMass[kk].Vx;
    CMass[kk].Y+=dt*CMass[kk].Vy;
    CMass[kk].Z+=dt*CMass[kk].Vz;

//-------- Вычисление угла Phi --------//
    RPoint phi;
    AngleSpeed(kk,phi);
    phi.X=dt*phi.X;
    phi.Y=dt*phi.Y;
    phi.Z=dt*phi.Z;
    //  Вращение агрегата посредством преобразования координат с помощью---//
                         //---- матрицы вращения ---//

    i=Pagregat[kk];
    while (i)
    {
      Pi=Npat(i);
      Pi->X+=dt*(CMass[kk].Vx+dVx);
      Pi->Y+=dt*(CMass[kk].Vy+dVy);
      Pi->Z+=dt*(CMass[kk].Vz+dVz);
      dX=Pi->X-CMass[kk].X;
      dY=Pi->Y-CMass[kk].Y;
      Pi->X=CMass[kk].X+dX*cos(phi.Z)-dY*sin(phi.Z);
      Pi->Y=CMass[kk].Y+dY*cos(phi.Z)+dX*sin(phi.Z);
      dY=Pi->Y-CMass[kk].Y;
      dZ=Pi->Z-CMass[kk].Z;
      Pi->Y=CMass[kk].Y+dY*cos(phi.X)-dZ*sin(phi.X);
      Pi->Z=CMass[kk].Z+dZ*cos(phi.X)+dY*sin(phi.X);

      dX=Pi->X-CMass[kk].X;
      dZ=Pi->Z-CMass[kk].Z;
      Pi->X=CMass[kk].X+dX*cos(phi.Y)-dZ*sin(phi.Y);
      Pi->Z=CMass[kk].Z+dZ*cos(phi.Y)+dX*sin(phi.Y);
      i=ConPat[i];
     }
   }
}

// Записать состояние агрегатов в файл

void Save()
{
  char fdat[128]; fdat[0]=0;
  strcpy(fdat,PathToDat()); strcat(fdat,"fractal.dat");
  _fmode=O_BINARY;
  int f=creat(fdat,S_IWRITE);
  for (Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  write (f, Pi, sizeof(Particle));
  write (f, CMass, sizeof(CMass));
  write (f, Pagregat, sizeof(Pagregat));
  write (f, ConPat, sizeof(ConPat));
  write (f, &s, sizeof(s));
  write (f, &dt, sizeof(t));
  write (f, &t, sizeof(t));
  close (f);

  fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"fract.dat");
  FILE *F=fopen(fdat,"w+");
  for(int i=1;i!=s;i++)
  { int j=Pagregat[i];
        while(j!=0)
        { Pj=Npat(j);
          fprintf(F,"%+e    %+e    %+e\xD\n",Pj->X,Pj->Y,Pj->Z);
          j=ConPat[j];
        }
        fprintf(F,"\xD\n");
  }
  fclose(F);

  fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"frsp.dat");
  FILE *F1=fopen(fdat,"w+");
  for (Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  fprintf(F1,"%+e    %+e    %+e\xD\n",Pi->X,Pi->Y,Pi->Z);
  fclose(F1);
}

void Load()
{ _fmode=O_BINARY;
  char fdat[128];fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"fractal.dat");
  int f=open(fdat,S_IREAD);

  TempP=new Particle;
  for(Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  {  TempP->next=Pi->next;
     TempP->pred=Pi->pred;
     read(f,Pi,sizeof(Particle));
     Pi->next=TempP->next;
     Pi->pred=TempP->pred;
  }
  delete TempP;

  read (f,CMass,sizeof(CMass));
  read (f,Pagregat,sizeof(Pagregat));
  read (f,ConPat,sizeof(ConPat));
  read (f,&s,sizeof(s));
  read (f,&dt,sizeof(t));
  read (f,&t,sizeof(t));
  close (f);
}

int NumPatOutAgr()
{ int i=0;
  for(Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
  if (Pi->stop) i++;
  return i;
}

float tpa;
void Fr_Model()
{ unsigned long mb=farcoreleft();
  struct ffblk ffblk;
  char fdat[128];
  int CountStop=0;
  fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"numpat.dat");
  _fmode=O_TEXT;
  FILE *cl;
  ch='n';
  int ti=0;
  DX=0; DY=0;
  t=0;s=1;
  PaletteData c,nm;
  palget(nm,0,15);
  for(int i=0;i<64;i++) { c[i+16].r=((i*i)/64); c[i+16].g=0; c[i+16].b=0;}
  for(i=0;i<64;i++) { c[i+64+16].r=((i*i)/64); c[i+64+16].g=0; c[i+64+16].b=((i*i)/64);}
  for(i=0;i<64;i++) { c[i+128+16].r=0; c[i+128+16].g=0; c[i+128+16].b=((i*i)/64);}
  palcopy(nm,c,0,15);
  InitGraph(-Xmax,-Ymax, Xmax,Ymax);
  OldRX=rangeX; OldRY=rangeY;
  KeyInt = getvect(9);
  setvect(9,ScPict);
  randomize();

  InitAgr();
  MakeArray(N);
  if (needLoad) Load(); else InitParticle();

  palset(c,0,255);
  BallinHeap();
  do
  { if (needCulon) CulonForce();
    if (needVDVaalse)
    { VanDerVaalce();
      for (Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
      { Pi->Fx+=Pi->VanX;
        Pi->Fy+=Pi->VanY;
        Pi->Fz+=Pi->VanZ;
      }
    }

    for (Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
    for (Pj=Pi->next;Pj!=NULL;Pj=Pj->next)
    { float dist_ij=pow(Pi->X-Pj->X,2)+pow(Pi->Y-Pj->Y,2)+pow(Pi->Z-Pj->Z,2);
      if (dist_ij<=pow((Pi->R+Pj->R),2)) UnitPaticle(Pi,Pj);
    }
    AgrForces();
    MovePart();
    switch (ch)
    { case 'n':{ ShowPicture(); break;}
      case 'h':{ HiResPict();break;}
    }
    if (NumPatOutAgr()==N/2) tpa=t;
//   ti++;
//  if (ti==200) ch='q';
    CountStop=0;
    for (Pi=FirstPat;Pi!=NULL;Pi=Pi->next)
    {  if (Pi->stop) CountStop=CountStop+1;
    }
    printf("CountStop = %d\n",CountStop);
    if (CountStop>=45)  ch='q';

    t=t+dt;
    if (kbhit()) ch=getch();
 } while(ch!='q');

 getch();
 restext();

 if (abs(findfirst(fdat,&ffblk,FA_RDONLY)))
    cl=fopen(fdat,"wt");
    else cl=fopen(fdat,"at");
    fprintf(cl,"%d   %e\n",NumPatOutAgr()+(s-1),t);
    fclose(cl);

 fdat[0]=0;
 strcpy(fdat,PathToDat());strcat(fdat,"tpa.dat");
 if ((int)(tpa/dt))
 { _fmode=O_TEXT;
   cl=fopen(fdat,"wt");
   fprintf(cl,"%e\n",tpa);
   fclose(cl);
 }

 if (needSave) Save();
 setvect(9,KeyInt);
 BalloffHeap();
 DistroyArray();
 gotoxy(20,13);
 printf("Aggregation time = %.3e\n",t);
 gotoxy(1,20);
 printf("memory lost %lu\n",mb-farcoreleft());
 getch();
}