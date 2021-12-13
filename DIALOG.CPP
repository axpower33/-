#define Uses_TColoredText
#define Uses_TDialog
#define Uses_TDeskTop
#define Uses_TProgram
#define Uses_TApplication
#define Uses_TObject
#define Uses_TInputLine
#define Uses_TLabel
#define Uses_THistory
#define Uses_TRect
#define Uses_TCheckBoxes
#define Uses_TButton
#define Uses_MsgBox
#define Uses_TSItem
#define Uses_TEditor
#define Uses_TRadioButtons

#include <tv.h>
#include "frshell.h"
#include <stdlib.h>
#include <stdarg.h>
#include <strstrea.h>
#include <iomanip.h>

ushort execDialog( TDialog *d, void *data )
{
		TView *p = TProgram::application->validView( d );
		if( p == 0 )
				return cmCancel;
		else
				{
				if( data != 0 )
						p->setData( data );
				ushort result = TProgram::deskTop->execView( p );
				if( result != cmCancel && data != 0 )
						p->getData( data );
				TObject::destroy( p );
				return result;
				}
}

TDialog *createFindDialog()
{
        TDialog *d = new TDialog( TRect( 0, 0, 38, 12 ), "Find" );

        d->options |= ofCentered;

        TInputLine *control = new TInputLine( TRect( 3, 3, 32, 4 ), 80 );
        d->insert( control );
        d->insert(
				new TLabel( TRect( 2, 2, 15, 3 ), "~T~ext to find", control ) );
		d->insert(
                new THistory( TRect( 32, 3, 35, 4 ), control, 10 ) );

        d->insert( new TCheckBoxes( TRect( 3, 5, 35, 7 ),
                new TSItem( "~C~ase sensitive",
				new TSItem( "~W~hole words only", 0 ))));

        d->insert(
                new TButton( TRect( 14, 9, 24, 11 ), "O~K~", cmOK, bfDefault ) );
        d->insert(
                new TButton( TRect( 26, 9, 36, 11 ), "Cancel", cmCancel, bfNormal ) );

		d->selectNext( False );
        return d;
}

TDialog *createReplaceDialog()
{
        TDialog *d = new TDialog( TRect( 0, 0, 40, 16 ), "Replace" );

        d->options |= ofCentered;

        TInputLine *control = new TInputLine( TRect( 3, 3, 34, 4 ), 80 );
        d->insert( control );
		d->insert(
                new TLabel( TRect( 2, 2, 15, 3 ), "~T~ext to find", control ) );
        d->insert( new THistory( TRect( 34, 3, 37, 4 ), control, 10 ) );

        control = new TInputLine( TRect( 3, 6, 34, 7 ), 80 );
		d->insert( control );
        d->insert( new TLabel( TRect( 2, 5, 12, 6 ), "~N~ew text", control ) );
        d->insert( new THistory( TRect( 34, 6, 37, 7 ), control, 11 ) );

        d->insert( new TCheckBoxes( TRect( 3, 8, 37, 12 ),
                new TSItem("~C~ase sensitive",
                new TSItem("~W~hole words only",
				new TSItem("~P~rompt on replace",
                new TSItem("~R~eplace all", 0 ))))));

        d->insert(
                new TButton( TRect( 17, 13, 27, 15 ), "O~K~", cmOK, bfDefault ) );
		d->insert( new TButton( TRect( 28, 13, 38, 15 ),
                                                        "Cancel", cmCancel, bfNormal ) );

	d->selectNext( False );
	return d;
}

TDialog* DlgGrowth()
{ TView *control;
  TDialog* dlg = new TDialog(TRect(15, 0, 55, 22), "Initial Conditions");
  dlg->options |= ofCentered;

  control = new TInputLine(TRect(21, 2, 30, 3), 4);
  dlg->insert(control);
  dlg->insert(new TLabel(TRect(1, 2, 18, 3), "Num of Particles", control));

  control = new TInputLine(TRect(21, 4, 30, 5), 8);
  dlg->insert(control);
  dlg->insert(new TLabel(TRect(1, 4, 18, 5), "Step On Time", control));

  control = new TInputLine(TRect(21, 6, 30, 7), 8);
  dlg->insert(control);
  dlg->insert(new TLabel(TRect(1, 6, 18, 7), "Rmin of Particle", control));

  control = new TInputLine(TRect(21, 8, 30, 9), 8);
  dlg->insert(control);

  dlg->insert(new TLabel(TRect(1, 8, 18, 9), "Rmax of Particle", control));

  control = new TInputLine(TRect(21, 10, 30, 11), 8);
  dlg->insert(control);

  dlg->insert(new TLabel(TRect(1, 10, 18, 11), "Rmid of Particle", control));

  control = new TInputLine(TRect(21, 12, 30, 13), 8);
  dlg->insert(control);

  dlg->insert(new TLabel(TRect(1, 12, 18, 13), "Free Walk Length ", control));

  control = new TInputLine(TRect(21, 14, 30, 15), 8);
  dlg->insert(control);

  dlg->insert(new TLabel(TRect(1, 14, 19, 15), "Density Of Medium", control));

  control = new TInputLine(TRect(21, 16, 30, 17), 8);
  dlg->insert(control);

  dlg->insert(new TLabel(TRect(1, 16, 13, 17), "Temperature", control));

  control = new TInputLine(TRect(21, 18, 30, 19), 8);
  dlg->insert(control);

  dlg->insert(new TLabel(TRect(1, 18, 13, 19), "Max Charge ", control));

  control = new TButton(TRect(3, 20, 13, 22), "O~k~", cmOK, bfNormal);
  dlg->insert(control);

  control = new TButton(TRect(15, 20, 25, 22), "R~u~n", cmRunM,bfDefault);
  dlg->insert(control);

  control = new TButton(TRect(27, 20, 37, 22), "C~a~ncel", cmCancel, bfNormal);
  dlg->insert(control);

  control = new TStaticText(TRect(31, 4, 32, 5), "c\n");
  dlg->insert(control);

  control = new TStaticText(TRect(31, 6, 33, 7), "cm\n"
				 "\n");
  dlg->insert(control);

  control = new TStaticText(TRect(31, 8, 33, 9), "cm");
  dlg->insert(control);

  control = new TStaticText(TRect(31, 10, 33, 11), "cm");
  dlg->insert(control);

  control = new TStaticText(TRect(31, 12, 33, 13), "cm\n");
  dlg->insert(control);

  control = new TStaticText(TRect(31, 14, 37, 15), "g/cm^3\n");
  dlg->insert(control);

  control = new TStaticText(TRect(31, 16, 32, 17), "K");
  dlg->insert(control);
  dlg->selectNext(False);
  return dlg;
}

TDialog* VelDstr()
{ TView *control;
  TDialog* dlg = new TDialog(TRect(12, 4, 63, 11), "Velosity Distributions");
  dlg->options |= ofCentered;
  control = new TRadioButtons(TRect(2, 2, 38, 5),
  new TSItem("Maxwell Distribution",
  new TSItem("Spontanuous Distribution",
  new TSItem("Initial Velosities is set to 0", 0))));
  dlg->insert(control);

  control = new TButton(TRect(39, 2, 49, 4), "C~a~nsel", cmCancel, bfDefault);
  dlg->insert(control);

  control = new TButton(TRect(39, 4, 49, 6), "O~k~", cmOK, bfNormal);
  dlg->insert(control);

  dlg->selectNext(False);

  return dlg;
}

TDialog* ProgOpt()
{
TView *control;
TDialog* dlg = new TDialog(TRect(0, 0, 40, 10), "Program Options");

dlg->options |= ofCentered;
control = new TCheckBoxes(TRect(2, 3, 27, 5),
new TSItem("Load \"...\\fract.dat\"",
new TSItem("Save \"...\\fract.dat\"", 0)));
dlg->insert(control);
dlg->insert(new TLabel(TRect(1, 2, 26, 3), "Load/Save Program State ", control));

control = new TInputLine(TRect(2, 7, 25, 8), 128);
dlg->insert(control);
dlg->insert(new THistory(TRect(24, 7, 27, 8), (TInputLine*)control, 5));
dlg->insert(new TLabel(TRect(1, 6, 22, 7), "Dir. for \"fract.dat\"", control));

control = new TButton(TRect(28, 3, 38, 5), "C~a~ncel", cmCancel, bfNormal);
dlg->insert(control);

control = new TButton(TRect(28, 7, 38, 9), "O~k~", cmOK, bfDefault);
dlg->insert(control);

dlg->selectNext(False);

return dlg;
}

TDialog* IFrDim()
{
TView *control;
TDialog* dlg = new TDialog(TRect(0, 0, 45, 11), "Initial Conditions");
dlg->options |= ofCentered;

control = new TInputLine(TRect(21, 2, 28, 3), 3);
dlg->insert(control);
dlg->insert(new THistory(TRect(28, 2, 31, 3), (TInputLine*)control, 5));
dlg->insert(new TLabel(TRect(1, 2, 21, 3), "Number Of Divisions", control));

control = new TInputLine(TRect(21, 4, 28, 5), 3);
dlg->insert(control);
dlg->insert(new THistory(TRect(28, 4, 31, 5), (TInputLine*)control, 5));
dlg->insert(new TLabel(TRect(1, 4, 20, 5), "Radius Of Division", control));

control = new TInputLine(TRect(21, 6, 28, 7), 4);
dlg->insert(control);
dlg->insert(new TLabel(TRect(1, 6, 19, 7), "Norm of Frac.Dim.", control));

control = new TStaticText(TRect(29, 6, 41, 7), "range <0..1>");
dlg->insert(control);

control = new TInputLine(TRect(21, 8, 28, 9), 5);
dlg->insert(control);
dlg->insert(new TLabel(TRect(1, 8, 10, 9), "Box Norm", control));
control = new TStaticText(TRect(29, 8, 41, 9), "range <0..1>");
dlg->insert(control);

control = new TButton(TRect(31, 2, 41, 4), "O~k~", cmOK, bfDefault);
dlg->insert(control);

control = new TButton(TRect(31, 4, 41, 6), "C~a~ncel",cmCancel, bfNormal);
dlg->insert(control);

dlg->selectNext(False);
return dlg;
}

TDialog* ISp3d()
{
TView *control;

TDialog* dlg = new TDialog(TRect(0, 0, 47, 15), "Dialog Design");
dlg->options |= ofCentered;

control = new TStaticText(TRect(2, 3, 19, 4), "Frequancy  Range\n");
dlg->insert(control);

control = new TInputLine(TRect(25, 2, 34, 3), 11);
dlg->insert(control);

  dlg->insert(new TLabel(TRect(18, 2, 24, 3), "from", control));

control = new TInputLine(TRect(25, 4, 34, 5), 11);
dlg->insert(control);

  dlg->insert(new TLabel(TRect(18, 4, 24, 5), "to", control));

control = new TStaticText(TRect(2, 9, 20, 10), "Length Wave Range ");
dlg->insert(control);

control = new TInputLine(TRect(25, 6, 34, 7), 8);
dlg->insert(control);

  dlg->insert(new TLabel(TRect(1, 6, 24, 7), "Frequancy Width", control));

control = new TInputLine(TRect(25, 8, 34, 9), 8);
dlg->insert(control);

  dlg->insert(new TLabel(TRect(18, 8, 24, 9), "from", control));

control = new TInputLine(TRect(25, 10, 34, 11), 8);
dlg->insert(control);

  dlg->insert(new TLabel(TRect(18, 10, 24, 11), "to", control));



control = new TButton(TRect(35, 2, 45, 4), "O~k~", cmOK, bfDefault);
dlg->insert(control);

control = new TButton(TRect(35, 6, 45, 8), "C~a~ncel", cmCancel, bfNormal);
dlg->insert(control);

control = new TInputLine(TRect(25, 12, 34, 13), 8);
dlg->insert(control);

  dlg->insert(new TLabel(TRect(1, 12, 24, 13), "Number *.gra file", control));

dlg->selectNext(False);
 return dlg;
}


TDialog* dlgPatInt()
{
TView *control;
TDialog* dlg = new TDialog(TRect(22, 7, 58, 15), "Particle's Interaction");

control = new TCheckBoxes(TRect(2, 2, 34, 4),
  new TSItem("Culon Forces Interaction",
  new TSItem("Van Der Vaalse Interaction", 0)));
dlg->insert(control);

control = new TButton(TRect(24, 5, 34, 7), "O~k~", cmOK, bfDefault);
dlg->insert(control);

control = new TButton(TRect(2, 5, 12, 7), "C~a~ncel", cmCancel, bfNormal);
dlg->insert(control);

dlg->selectNext(False);
return dlg;
}

