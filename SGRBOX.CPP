#include<math.h>;
#include "svgacc.h"
#include<stdlib.h>;

float GetminX;
float GetminY;
float GetmaxX;
float GetmaxY;
float GetScX;
float GetScY;
float rangeX;
float rangeY;
float rangeR;
float rng;

void far InitGraph (float xmin, float ymin,float xmax, float ymax)
{
   if ( !whichvga() || (whichmem() < 512))  exit(1);
   res640();
   GetminX=xmin;
   GetminY=ymin;
   GetmaxX=xmax;
   GetmaxY=ymax;
   GetScX=(GetmaxX-GetminX);
   GetScY=(GetmaxY-GetminY);
   rangeX=maxx/GetScX;
   rangeY=maxy/GetScY;
   rangeR=sqrt(pow(rangeX,2)+pow(rangeY,2));
}

int round(float v)
{
 return(v+0.5);
}

void far Circle (int clr, float x,float y, float r)
{
  drwellipse(SET,clr,round((x-GetminX)*rangeX),round(rangeY*(GetScY-y+GetminY)),
	     round(r*rangeX), round(r*rangeY));
}

void Line(int clr,float x1,float y1,float x2,float y2)
{
 drwline(SET,clr,round((x1-GetminX)*rangeX),round((GetScY-y1+GetminY)*rangeY),
	 round((x2-GetminX)*rangeX),round((GetScY-y2+GetminY)*rangeY));
}

void Rectangle(int clr,float x1,float y1,float x2,float y2)
{
   drwbox(SET,clr,round((x1-GetminX)*rangeX),round((GetScY-y1+GetminY)*rangeY),
	 round((x2-GetminX)*rangeX),round((GetScY-y2+GetminY)*rangeY));
}
#define BLACK  0
#define BLUE   1
#define WHITE  15

void OutTextXY (int clr,float x, float y, char ss[100])
{
  drwstring (AND,BLACK,WHITE,ss,round((x-GetminX)*rangeX), round((GetScY-y+GetminY)*rangeY));
  drwstring (OR,clr,BLACK,ss,round((x-GetminX)*rangeX), round((GetScY-y+GetminY)*rangeY));
}
void FillEllipse(int clr,float x,float y, float xradius, float yradius)
{ drwfillellipse(SET,clr,round((x-GetminX)*rangeX),
	   round((GetScY-y+GetminY)*rangeY),
	   round(xradius*rangeX),round(yradius*rangeY));
}
/*void Circl (float x,float y,float r)
{
  circle (round((x-GetminX)*rangeX),round(rangeY*(GetScY-y+GetminY)),
										  round(r*rangeR));
}

void PieSlice (float x,float y, int StAngle, int EndAngle, float r)
{
 pieslice (round((x-GetminX)*rangeX),round(rangeY*(GetScY-y+GetminY)),
 StAngle, EndAngle,round(r*rangeR));
}



void MoveTo (float x,float y)
{
   moveto (round((x-GetminX)*rangeX),
    round((GetScY-y+GetminY)*rangeY));
}

void LineTo (float x,float y)
{
   lineto (round((x-GetminX)*rangeX),
		  round((GetScY-y+GetminY)*rangeY));
}
void PutImage(float x,float y, void far *bitmap, int op)
{
 putimage (round((x-GetminX)*rangeX),
   round((GetScY-y+GetminY)*rangeY), bitmap, op);
}

void FloodFill(float x, float y, int Border)
{ floodfill (round((x-GetminX)*rangeX),
	     round((GetScY-y+GetminY)*rangeY), Border);
}
*/

float oldminX;
float oldminY;
float oldmaxX;
float oldmaxY;
float oldScX;
float oldScY;

void RangeBox(float r)
{ rng=r;
  oldminX = GetminX;
  oldminY = GetminY;
  oldmaxX = GetmaxX;
  oldmaxY = GetmaxY;
  oldScX = GetScX;
  oldScY = GetScY;
  GetminX-=rng*GetScX;
  GetminY-=rng*GetScY;
  GetmaxX+=rng*GetScX;
  GetmaxY+=rng*GetScY;
  GetScX=(GetmaxX-GetminX);
  GetScY=(GetmaxY-GetminY);
  rangeX=maxx/GetScX;
  rangeY=maxy/GetScY;
}

void Axes(int clr)
{
 Line(clr,oldminX,0,oldmaxX,0);
 Line(clr,0,oldminY,0,oldmaxY);
 Line(clr,0.95*oldmaxX,0.01*oldmaxY,oldmaxX,0);
 Line(clr,0.01*oldmaxX,0.95*oldmaxY,0,oldmaxY);
 Line(clr,0.95*oldmaxX,-0.01*oldmaxY,oldmaxX,0);
 Line(clr,-0.01*oldmaxX,0.95*oldmaxY,0,oldmaxY);
}

void Box(int Count)
{
 char ss[50];
 int i;
 float Yval,Xval;
 fillpage(WHITE);
 Rectangle(BLUE,oldminX,oldminY,oldmaxX,oldmaxY);
 for (i=0;i<Count+1;i++)
 {
  Yval=oldminY+oldScY*i/Count;
  gcvt(Yval,4,ss);
  OutTextXY(BLUE,oldminX-rng*oldScX,Yval+0.05*rng*oldScY,ss);
  Xval=oldminX+oldScX*i/Count;
  gcvt(Xval,4,ss);

  OutTextXY(BLUE,Xval-0.03*rng*oldScX,oldminY-0.3*rng*oldScY,ss);

  Line(BLUE,Xval,oldminY-0.1*rng*oldScY,Xval,oldminY);
  Line(BLUE,oldminX-0.075*rng*oldScX,Yval,oldminX,Yval);
 }
}
