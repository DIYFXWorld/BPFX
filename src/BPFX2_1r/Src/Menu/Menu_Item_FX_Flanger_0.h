#ifndef Menu_Item_FX_Flanger_0_h_
#define Menu_Item_FX_Flanger_0_h_

#include "Myutil.h"
#include "Menu_Item_Interface.h"
#include "FX_Flanger_0.h"

struct Menu_Item_FX_Flanger_0 : public Menu_Item_Interface
{
	int8_t	Rate, Depth, Mix_Level, Time, Feedback;

	Menu_Item_FX_Flanger_0():
		Rate( 50 ), Depth( 50 ), Mix_Level( 50 ), Time( 10 ), Feedback( 70 )
	{
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

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_Flanger_0;
		Set_Rate( Rate );
		Set_Depth( Depth );
		Set_Time( Time );
		Set_Feedback( Feedback );
		Set_Mix_Level( Mix_Level );
		return pFX;
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'R':	Set_Rate( v ); 			break;
			case 'D': Set_Depth( v );			break;
			case 'T': Set_Time( v );			break;
			case 'F': Set_Feedback( v );	break;
			case 'M': Set_Mix_Level( v );	break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Rate(R)    : " );	println( Rate );
		print( "Depth(D)   : " );	println( Depth );
		print( "Time(T)    : " );	println( Time );
		print( "Feedback(F): " );	println( Feedback );
		print( "Mix(M)     : " );	println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Flanger(fc20kHz)" );
	}
};

#endif
