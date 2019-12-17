#ifndef MENU_Analog_DELAY_H_
#define MENU_Analog_DELAY_H_

#include	"Menu_Interface.h"
#include	"FX_Process.h"
#include	"Common.h"

struct Menu_Analog_Delay : public Menu_Interface
{
	int	Time, Feedback, Mix_Level;

	Menu_Analog_Delay():
		Time( 30 ), Feedback( 70 ), Mix_Level( 50 )
	{
	}

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Analog_Delay; }

	void Set_Time( int v )
	{
		Time = v;
		FX->Set_Control_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Feedback( int v )
	{
		Feedback = v;
		FX->Set_Control_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Control_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Analog_Delay );
		Set_Time( Time );
		Set_Feedback( Feedback );
		Set_Mix_Level( Mix_Level );
		FX->Reset = true;
	}

	virtual void Set_Control( char c, int v )
	{
		switch( c )
		{
			case 'T': Set_Time( v ); 			break;
			case 'F': Set_Feedback( v );	break;
			case 'M': Set_Mix_Level( v );	break;
		}
	}

	virtual void Print_Paramaters()
	{
		printf( "Time(T)    : %d\n", Time );
		printf( "Feedback(F): %d\n", Feedback );
		printf( "Mix(M)     : %d\n", Mix_Level );
	}

	virtual void Print_Caption()
	{
		printf( "Analog Delay" );
	}
};

#endif /* MENU_Analog_DELAY_H_ */
