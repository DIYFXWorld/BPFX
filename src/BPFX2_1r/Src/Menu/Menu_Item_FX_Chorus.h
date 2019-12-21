#ifndef Menu_Item_FX_Chorus_h_
#define Menu_Item_FX_Chorus_h_

#include "Myutil.h"
#include "Menu_Item_Interface.h"
#include "vcp_mini.h"
#include "FX_Chorus.h"

struct Menu_Item_FX_Chorus : public Menu_Item_Interface
{
	int8_t	Rate, Depth, Mix_Level;

	Menu_Item_FX_Chorus():
		Rate( 30 ), Depth( 30 ), Mix_Level( 70 )
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

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		pFX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_Chorus;
		Set_Rate( Rate );
		Set_Depth( Depth );
		Set_Mix_Level( Mix_Level );
		return pFX;
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'R': Set_Rate( v ); 			break;
			case 'D': Set_Depth( v );			break;
			case 'M': Set_Mix_Level( v );	break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Rate(R) : " );	println( Rate );
		print( "Depth(D): " );	println( Depth );
		print( "Mix(M)  : " );	println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Chorus(fc10kHz)" );
	}
};

#endif
