#ifndef Menu_Item_FX_Delay_h_
#define Menu_Item_FX_Delay_h_

#include "Myutil.h"
#include "Menu_Item_Interface.h"
#include "vcp_mini.h"
#include "FX_Delay.h"

struct Menu_Item_FX_Delay : public Menu_Item_Interface
{
	int8_t	Time, Feedback, Mix_Level;

	Menu_Item_FX_Delay():
		Time( 99 ), Feedback( 50 ), Mix_Level( 70 )
	{
	}

	void Set_Time( int v )
	{
		Time = v;
		pFX->Set_Param_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Feedback( int v )
	{
		Feedback = v;
		pFX->Set_Param_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		pFX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_Delay;
		Set_Time( Time );
		Set_Feedback( Feedback );
		Set_Mix_Level( Mix_Level );
		return pFX;
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
		print( "Delay(fc10kHz)" );
	}
};

#endif
