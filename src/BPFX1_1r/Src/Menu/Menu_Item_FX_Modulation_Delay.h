#ifndef Menu_Item_FX_Modulation_Delay_h_
#define Menu_Item_FX_Modulation_Delay_h_

#include "Myutil.h"
#include "Menu_Item_Interface.h"
#include "FX_Modulation_Delay.h"

struct Menu_Item_FX_Modulation_Delay : public Menu_Item_Interface
{
	int8_t	Time, Feedback, Mix_Level, Rate, Depth;

	Menu_Item_FX_Modulation_Delay():
		Time( 99 ), Feedback( 30 ), Mix_Level( 50 ), Rate( 10 ), Depth( 80 )
	{
	}

	void Set_Time( int v )
	{
		Time = v;
		pFX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Feedback( int v )
	{
		Feedback = v;
		pFX->Set_Param_3( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		pFX->Set_Param_4( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Rate( int v )
	{
		Rate = v;
		pFX->Set_Param_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Depth( int v )
	{
		Depth = v;
		pFX->Set_Param_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_Modulation_Delay;
		Set_Time( Time );
		Set_Feedback( Feedback );
		Set_Mix_Level( Mix_Level );
		Set_Rate( Rate );
		Set_Depth( Depth );
		return pFX;
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
		print( "Modulation Delay(fc2kHz)" );
	}
};

#endif
