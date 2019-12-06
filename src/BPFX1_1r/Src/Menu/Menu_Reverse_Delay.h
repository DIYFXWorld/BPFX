#ifndef MENU_Reverse_DELAY_H_
#define MENU_Reverse_DELAY_H_

#include	"..\FX_Process.h"
#include	"Myutil.h"
#include	"Menu_Interface.h"

struct Menu_Reverse_Delay : public Menu_Interface
{
	int8_t	Time, Feedback, Mix_Level;

	Menu_Reverse_Delay():
		Time( 99 ), Feedback( 0 ), Mix_Level( 99 )
	{
	}

	void Set_Time( int v )
	{
		Time = v;
		FX->Set_Param_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Feedback( int v )
	{
		Feedback = v;
		FX->Set_Param_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Reverse_Delay );
		Set_Time( Time );
		Set_Feedback( Feedback );
		Set_Mix_Level( Mix_Level );
	}

	virtual void Set_Param( char c, int v )
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
		print( "Time(T)    : " );	println( Time );
		print( "Feedback(F): " );	println( Feedback );
		print( "Mix(M)     : " );	println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Reverse Delay" );
	}
};

#endif /* MENU_Reverse_DELAY_H_ */
