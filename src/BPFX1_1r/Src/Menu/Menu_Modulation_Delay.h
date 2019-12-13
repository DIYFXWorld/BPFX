#ifndef MENU_Modulation_DELAY_H_
#define MENU_Modulation_DELAY_H_

#include	"FX_Process.h"
#include	"Myutil.h"
#include	"Menu_Interface.h"

struct Menu_Modulation_Delay : public Menu_Interface
{
	int8_t	Time, Feedback, Mix_Level, Rate, Depth;

	Menu_Modulation_Delay():
		Time( 30 ), Feedback( 70 ), Mix_Level( 50 ), Rate( 50 ), Depth( 50 )
	{
	}

	void Set_Time( int v )
	{
		Time = v;
		FX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Feedback( int v )
	{
		Feedback = v;
		FX->Set_Param_3( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Param_4( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Rate( int v )
	{
		Rate = v;
		FX->Set_Param_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Depth( int v )
	{
		Depth = v;
		FX->Set_Param_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Modulation_Delay );
		Set_Time( Time );
		Set_Feedback( Feedback );
		Set_Mix_Level( Mix_Level );
		Set_Rate( Rate );
		Set_Depth( Depth );
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'T': Set_Time( v ); 			break;
			case 'F': Set_Feedback( v );	break;
			case 'M': Set_Mix_Level( v );	break;
			case 'R': Set_Rate( v );			break;
			case 'D': Set_Depth( v );			break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Time(T)    : " );	println( Time );
		print( "Rate(R)    : " ); println( Rate );
		print( "Depth(D)   : " ); println( Depth );
		print( "Feedback(F): " ); println( Feedback );
		print( "Mix(M)     : " ); println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Modulation Delay" );
	}
};

#endif /* MENU_Modulation_DELAY_H_ */
