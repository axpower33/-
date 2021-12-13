#define Uses_TApplication
#define Uses_TEditWindow
#define Uses_TDeskTop
#define Uses_TRect
#define Uses_TEditor
#define Uses_TFileEditor
#define Uses_TFileDialog
#define Uses_TChDirDialog
#define Uses_MsgBox
#include <tv.h>
#include "fractal.h"
#include "frshell.h"
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
TEditWindow *clipWindow;
dataRec TdataRec;
dataVDstr TdataVDstr;
dataOpPr TdataOpPr;
dataInitFrDim TdataInitFrDim;
dataInitSp3d  TdataInitSp3d;
dataPatInt  TdataPatInt;

TEditWindow *TMyApp::openEditor( const char *fileName, Boolean visible )
{
  TRect r = deskTop->getExtent();
  TView *p = validView( new TEditWindow( r, fileName, wnNoNumber ) );
  if( !visible )  p->hide();
  deskTop->insert( p );
  return (TEditWindow *)p;
}

TMyApp::TMyApp() :
        TProgInit( TMyApp::initStatusLine,
                           TMyApp::initMenuBar,
                           TMyApp::initDeskTop
                         ) , TApplication()
{       TCommandSet ts;
        ts.enableCmd( cmSave );
        ts.enableCmd( cmSaveAs );
        ts.enableCmd( cmCut );
        ts.enableCmd( cmCopy );
        ts.enableCmd( cmPaste );
        ts.enableCmd( cmClear );
        ts.enableCmd( cmUndo );
        ts.enableCmd( cmFind );
        ts.enableCmd( cmReplace );
        ts.enableCmd( cmSearchAgain );
        disableCommands( ts );

        TEditor::editorDialog = doEditDialog;
        clipWindow = openEditor( 0, False );
        if( clipWindow != 0 )
        {
          TEditor::clipboard = clipWindow->editor;
          TEditor::clipboard->canUndo = False;
        }
        strcpy(TdataRec.dt,"2e-6");
        strcpy(TdataRec.N,"50");
        strcpy(TdataRec.Rmin,"10e-7");
        strcpy(TdataRec.Rmax,"20e-7");
        strcpy(TdataRec.Rmid,"100e-7");
        strcpy(TdataRec.Tmshft,"1e-3");
        strcpy(TdataRec.DensAg,"10.5");
        strcpy(TdataRec.Tk,"300");
        strcpy(TdataRec.MaxQ,"30");
        TdataVDstr.VelDstr=0;
        TdataOpPr.LdSv=2;
        TdataPatInt.PatInt=1;
        strcpy(TdataOpPr.dir,"DAT2");
        strcpy(TdataInitFrDim.Kmax,"1");
        strcpy(TdataInitFrDim.Scr,"6");
        strcpy(TdataInitFrDim.Koeff,"0.5");
        strcpy(TdataInitFrDim.rng,"0.17");

        strcpy(TdataInitSp3d.Xmn,"-5e17");
        strcpy(TdataInitSp3d.Xmx,"5e17");
        strcpy(TdataInitSp3d.del,"5e16");
        strcpy(TdataInitSp3d.Lmn,"4e-8");
        strcpy(TdataInitSp3d.Lmx,"8e-7");
        strcpy(TdataInitSp3d.Cg,"1");

}

void TMyApp::fileOpen()
{
        char fileName[MAXPATH];
        strcpy( fileName,"*.DAT*" );

        if( execDialog( new TFileDialog( "*.DAT*", "Open file",
        "~N~ame", fdOpenButton, 100 ), fileName) != cmCancel )
        openEditor( fileName, True );
}

void TMyApp::fileNew()
{
   openEditor( 0, True );
}

void TMyApp::changeDir()
{
   execDialog( new TChDirDialog( cdNormal, 0 ), 0 );
}

void TMyApp::dosShell()
{
        suspend();
        system("cls");
        cout << "Type EXIT to return...";
        system( getenv( "COMSPEC"));
        resume();
        redraw();
}

void TMyApp::showClip()
{
  clipWindow->select();
  clipWindow->show();
}

void TMyApp::tile()
{
  deskTop->tile( deskTop->getExtent() );
}

void TMyApp::cascade()
{
  deskTop->cascade( deskTop->getExtent() );
}

int result;
void TMyApp::InitCond()
{ result=execDialog(DlgGrowth(),&TdataRec);}

void TMyApp::VDstr()
{ execDialog(VelDstr(),&TdataVDstr);}

void TMyApp::POpt()
{ execDialog(ProgOpt(),&TdataOpPr);}

void TMyApp::InitFrDim()
{ execDialog(IFrDim(),&TdataInitFrDim);}

void TMyApp::InitSp3d()
{ execDialog(ISp3d(),&TdataInitSp3d);}

void TMyApp::IPatInt()
{ execDialog(dlgPatInt(),&TdataPatInt);
}

char* PathToDat()
{int d=getdisk()+'A';
 char disk[4];
 *(int*)disk=d;
 strcat (disk, ":\\");
 char curdir[128];
 getcurdir(0,curdir);
 mkdir(TdataOpPr.dir);
 char path[128];
 path[0]=0;
 strcat(path,disk);
 strcat(path,curdir);
 strcat(path,"\\");
 strcat(path,TdataOpPr.dir);
 strcat(path,"\\");
 return path;
}

void LoadParam()
{ _fmode=O_BINARY;
  char fdat[128];fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"fparam.dat");
  int f=open(fdat,S_IREAD);
  read (f,&N,sizeof(N));
  read (f,&dt,sizeof(dt));
  read (f,&Rmin,sizeof(Rmin));
  read (f,&Rmax,sizeof(Rmax));
  read (f,&Rmid,sizeof(Rmid));
  read (f,&Tmshft,sizeof(Tmshft));
  read (f,&DensAg,sizeof(DensAg));
  read (f,&Tk,sizeof(Tk));
  read (f,&MaxQ,sizeof(MaxQ));
  read (f,&needCulon,sizeof(needCulon));
  read (f,&needVDVaalse,sizeof(needVDVaalse));
  close (f);
}

void SaveParam()
{ _fmode=O_BINARY;
  char fdat[128];fdat[0]=0;
  strcpy(fdat,PathToDat());strcat(fdat,"fparam.dat");
  int f=creat(fdat,S_IWRITE);
  write (f,&N,sizeof(N));
  write (f,&dt,sizeof(dt));
  write (f,&Rmin,sizeof(Rmin));
  write (f,&Rmax,sizeof(Rmax));
  write (f,&Rmid,sizeof(Rmid));
  write (f,&Tmshft,sizeof(Tmshft));
  write (f,&DensAg,sizeof(DensAg));
  write (f,&Tk,sizeof(Tk));
  write (f,&MaxQ,sizeof(MaxQ));
  write (f,&needCulon,sizeof(needCulon));
  write (f,&needVDVaalse,sizeof(needVDVaalse));
  close (f);
}

void TMyApp::Fr_Mod()
{ VDn=TdataVDstr.VelDstr;
  LdSv=TdataOpPr.LdSv;
  PatInt=TdataPatInt.PatInt;

  if ((bool)(!PatInt)) needCharge=false;else needCharge=true;
  needLoad = (bool)(LdSv&1);
  needSave = (bool)((LdSv&2)/2);
  if (needLoad) LoadParam();
  else
   { N=atoi(TdataRec.N);
     dt=atof(TdataRec.dt);
     Rmin=atof(TdataRec.Rmin);
     Rmax=atof(TdataRec.Rmax);
     Rmid=atof(TdataRec.Rmid);
     Tmshft=atof(TdataRec.Tmshft);
     DensAg=atof(TdataRec.DensAg);
     Tk=atoi(TdataRec.Tk);
     MaxQ=atoi(TdataRec.MaxQ);
     needCulon =(bool)(PatInt&1);
     needVDVaalse=(bool)((PatInt&2)/2);
     SaveParam();
  }
  Xmax=2*sqrt(N)*(2*Rmax+Rmid)*4/3/2;
  Ymax=2*sqrt(N)*(2*Rmax+Rmid)/2;
  Zmax=2*sqrt(N)*(2*Rmax+Rmid)*4/3/2;
  suspend();
  Fr_Model();
  resume();
  redraw();
}

void TMyApp::Fr_Dim()
{
 LoadParam();
 Xmax=sqrt(N)*(2*Rmax+Rmid)*4/3/2;
 Ymax=sqrt(N)*(2*Rmax+Rmid)/2;
 Zmax=sqrt(N)*(2*Rmax+Rmid)*4/3/2;

 Kmax=atoi(TdataInitFrDim.Kmax);
 ScR=atoi(TdataInitFrDim.Scr);
 Koeff=atof(TdataInitFrDim.Koeff);
 RNG=atof(TdataInitFrDim.rng);

 suspend();
 FracDim();
 resume();
 redraw();
}
void TMyApp::ShGenSp()
{
  Lmin=atof(TdataInitSp3d.Lmn);
  Lmax=atof(TdataInitSp3d.Lmx);
  suspend();
  GrSpGen();
  resume();
  redraw();
}

void TMyApp::DelSp()
{
 ushort l=messageBox("Do you really want to erase *.gra files?",
                          0x0003|mfYesButton|mfNoButton);
 if (l==cmYes) DelGrFiles();
}

void TMyApp::Sp3d()
{
 LoadParam();
 nn=3*N;
 Xmn=atof(TdataInitSp3d.Xmn);
 Xmx=atof(TdataInitSp3d.Xmx);
 del=atof(TdataInitSp3d.del);
 Lmin=atof(TdataInitSp3d.Lmn);
 Lmax=atof(TdataInitSp3d.Lmx);
 countCall=atof(TdataInitSp3d.Cg);

 suspend();
 if (!Spectr3d()) messageBox("Insufficiant memory to allocate array", 0x0401);
 resume();
 redraw();
}

void TMyApp::handleEvent( TEvent& event )
{
        if (event.what == evBroadcast &&
                event.message.command == 52) return;

        TApplication::handleEvent( event );
        if( event.what != evCommand )
                return;
        else
                switch( event.message.command )
                        {
                        case cmOpen:
                                 fileOpen();
                                 break;
                        case cmNew:
                                 fileNew();
                                 break;
                        case cmChangeDrct:
                                 changeDir();
                                 break;
                        case cmDosShell:
                                 dosShell();
                                 break;
                        case cmShowClip:
                                 showClip();
                                 break;
                        case cmTile:
                                 tile();
                                 break;
                        case cmCascade:
                                 cascade();
                                 break;
                        case cmIcom:
                                 InitCond();
                                 if (result==cmRunM) Fr_Mod();
                                 break;
                        case cmRunM:
                                 Fr_Mod();
                                 break;
                        case cmVdis:
                                 VDstr();
                                 break;
                        case cmPrOp:
                                 POpt();
                                 break;
                        case cmFrDim:
                                 Fr_Dim();
                                 break;
                        case cmInitFrDim:
                                 InitFrDim();
                                 break;
                        case cmSp3d:
                                 Sp3d();
                                 break;
                        case cmInitSp3d:
                                 InitSp3d();
                                 break;
                        case cmPatInt:
                                 IPatInt();
                                 break;
                        case cmGenSp:
                                 ShGenSp();
                                 break;
                        case cmDelSp:
                                 DelSp();
                                 break;
                        default:
                         return ;
                        }
        clearEvent(event);
}

int main()
{
        TMyApp FrShell;
        FrShell.run();
        return 0;
}

