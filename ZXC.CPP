void ball(int x,int y,int r,int c,v3 e)
{
  float _min=1e5, _max=-1e5;
  int i,j,k;
  float s,l;
  l=sqrt(e.x*e.x+e.y*e.y+e.z*e.z);
  for(i=-r;i!=r;i++)
  for(j=-r;j!=r;j++)
  {
	k=r*r-i*i-j*j;
	if (k>=0)
	{ s=(e.x*i-e.y*j+e.z*sqrt(k))/l;
	  if (s<_min) _min=s;
	  if (s>_max) _max=s;
	}
  }
  for(i=-r;i!=r;i++)
  for(j=-r;j!=r;j++)
  {	k=r*r-i*i-j*j;
	if (k>=0)
	{ s=(e.x*i-e.y*j+e.z*sqrt(k))/l;
	  drwpoint(SET,c+63*(1+s-_min)/(_max-_min+2),x+i,y+j);
	}
  }
}

void BallinHeap()
{ int i,r;
  v3 beam={0.3, 0.3,1};
  _fmode=O_BINARY;
  int f1=creat("pr.dat",S_IWRITE);
  int f2=creat("pb.dat",S_IWRITE);
  int f3=creat("pm.dat",S_IWRITE);

  if (FirstPat->q!=0)
  {  for(r=0;r<Rmx;r++)
     { ball(r,r,r,16,beam);
       i=(2*r+1)*(2*r+1)+4;
       pr[r]=(RasterBlock*)malloc(i);
       blkget(0,0,2*r,2*r,pr[r]);
       write(f1,pr[r],i);
       fillpage(0);
     }
     for(r=0;r<Rmx;r++)
     { ball(r,r,r,128+16,beam);
       i=(2*r+1)*(2*r+1)+4;
       pb[r]=(RasterBlock*)malloc(i);
       blkget(0,0,2*r,2*r,pb[r]);
       write(f2,pb[r],i);
       fillpage(0);
     }
  }else
   for(r=0;r<Rmx;r++)
   { ball(r,r,r,64+16,beam);
     i=(2*r+1)*(2*r+1)+4;
     pm[r]=(RasterBlock*)malloc(i);
     blkget(0,0,2*r,2*r,pm[r]);
     write(f3,pm[r],i);
     fillpage(0);
   }
   close(f1);
   close(f2);
   close(f3);
}


void ball(int x,int y,int r,int c,v3 e)
{
  float _min=1e5, _max=-1e5;
  int i,j,k;
  float s,l;
  l=sqrt(e.x*e.x+e.y*e.y+e.z*e.z);
  for(i=-r;i!=r;i++)
  for(j=-r;j!=r;j++)
  {
	k=r*r-i*i-j*j;
	if (k>=0)
	{ s=(e.x*i-e.y*j+e.z*sqrt(k))/l;
	  if (s<_min) _min=s;
	  if (s>_max) _max=s;
	}
  }
  for(i=-r;i!=r;i++)
  for(j=-r;j!=r;j++)
  {	k=r*r-i*i-j*j;
	if (k>=0)
	{ s=(e.x*i-e.y*j+e.z*sqrt(k))/l;
	  drwpoint(SET,c+63*(1+s-_min)/(_max-_min+2),x+i,y+j);
	}
  }
}

void maskball(int x,int y,int r)
{ int i,j;
  for (i=-r;i<r+1;i++)
  for (j=-r;j<r+1;j++)
  { int  k=r*r-i*i-j*j;
    if (k<0) drwpoint(SET,255,x+i,y+j);
    else drwpoint(SET,0,x+i,y+j);
  }
}