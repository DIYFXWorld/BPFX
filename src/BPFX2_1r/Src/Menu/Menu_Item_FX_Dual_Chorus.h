#ifndef Menu_Item_FX_Dual_Chorus_h_
#define Menu_Item_FX_Dual_Chorus_h_

#include "Myutil.h"
#include "Menu_Item_Interface.h"
#include "vcp_mini.h"
#include "FX_Dual_Chorus.h"

struct Menu_Item_FX_Dual_Chorus : public Menu_Item_Interface
{
	int8_t	Rate_0, Depth_0;
	int8_t	Rate_1, Depth_1;
	int8_t	Mix_Level;

	Menu_Item_FX_Dual_Chorus():
		Rate_0( 60 ), Depth_0( 20 ),
		Rate_1( 40 ), Depth_1( 20 ),
		Mix_Level( 70 )
	{
	}

	void Set_Rate_0( int v )
	{
		Rate_0 = v;
		pFX->Set_Param_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Depth_0( int v )
	{
		Depth_0 = v;
		pFX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Rate_1( int v )
	{
		Rate_1 = v;
		pFX->Set_Param_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Depth_1( int v )
	{
		Depth_1 = v;
		pFX->Set_Param_3( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		pFX->Set_Param_4( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_Dual_Chorus;
		Set_Rate_0( Rate_0 );
		Set_Depth_0( Depth_0 );
		Set_Rate_1( Rate_1 );
		Set_Depth_1( Depth_1 );
		Set_Mix_Level( Mix_Level );
		return pFX;
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'R': Set_Rate_0( v ); 		break;
			case 'D': Set_Depth_0( v );		break;
			case 'A': Set_Rate_1( v ); 		break;
			case 'E': Set_Depth_1( v );		break;
			case 'M': Set_Mix_Level( v );	break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Rate 1(R) : " );	println( Rate_0 );
		print( "Depth 1(D): " );	println( Depth_0 );
		print( "Rate 2(A) : " );	println( Rate_1 );
		print( "Depth 2(E): " );	println( Depth_1 );
		print( "Mix(M)  : " );	println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Dual Chorus" );
	}
};

#endif
