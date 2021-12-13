class far TEditWindow;

const
  cmOpen       = 100,
  cmNew        = 101,
  cmChangeDrct = 102,
  cmDosShell   = 103,
  cmCalculator = 104,
  cmShowClip   = 105,
  cmFrDim      = 201,

  cmIcom=203,
  cmVdis=204,
  cmPdat=205,
  cmVdat=206,
  cmPrOp=207,
  cmInitFrDim=208,
  cmRunM=cmYes,
  cmSp3d=209,
  cmInitSp3d=210,
  cmPatInt=211,
  cmGenSp=212,
  cmDelSp=213;

class TMyApp : public TApplication
{

public:

	TMyApp();

	virtual void handleEvent( TEvent& event );
	static TMenuBar *initMenuBar( TRect );
	static TStatusLine *initStatusLine( TRect );
	virtual void outOfMemory();
private:

	TEditWindow *openEditor( const char *fileName, Boolean visible );
	void fileOpen();
	void fileNew();
	void changeDir();
	void dosShell();
	void showClip();
	void tile();
	void cascade();
	void Fr_Mod();
	void InitCond();
	void VDstr();
	void POpt();
	void Fr_Dim();
	void InitFrDim();
	void Sp3d();
	void InitSp3d();
	void IPatInt();
	void ShGenSp();
	void DelSp();
};

extern TEditWindow *clipWindow;

struct dataRec {
 char N[4];
 char dt[8];
 char Rmin[8];
 char Rmax[8];
 char Rmid[8];
 char Tmshft[8];
 char DensAg[8];
 char Tk[8];
 char MaxQ[8];
};

struct dataVDstr {
  ushort VelDstr;
 };

struct dataOpPr  {
  ushort LdSv;
  char dir[10];
  };

struct dataInitFrDim {
  char Kmax[3];
  char Scr[3];
  char Koeff[4];
  char rng[5];
  };

struct dataInitSp3d
 {
  char Xmn[11];
  char Xmx[11];
  char del[8];
  char Lmn[8];
  char Lmx[8];
  char Cg[8];
  };

struct dataPatInt
{ushort PatInt;
};

extern dataRec TdataRec;
extern dataVDstr TdataVDstr;
extern dataOpPr TdataOpPr;
extern dataInitFrDim TdataInitFrDim;
extern dataInitSp3d TdataInitSp3d;
extern dataPatInt TdataPatInt;


ushort execDialog( TDialog *d, void *data );
TDialog *createFindDialog();
TDialog *createReplaceDialog();
TDialog *DlgGrowth();
TDialog *VelDstr();
TDialog *ProgOpt();
TDialog *IFrDim();
TDialog *ISp3d();
TDialog *dlgPatInt();

ushort doEditDialog( int dialog, ... );

