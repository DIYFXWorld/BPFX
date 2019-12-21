#ifndef Menu_Item_FX_Tremolo_h_
#define Menu_Item_FX_Tremolo_h_

#include "Myutil.h"
#include "Menu_Item_Interface.h"
#include "FX_Tremolo.h"

struct Menu_Item_FX_Tremolo : public Menu_Item_Interface
{
	int16_t	Rate, Depth, Mode;

	Menu_Item_FX_Tremolo():
		Rate( 50 ), Depth( 80 ), Mode( 1 )
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

	void Set_Mode( int v )
	{
		if( v == 1 || v == 2 )
		Mode = v;
		pFX->Set_Param_4( v );
	}

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_Tremolo;
		Set_Rate( Rate );
		Set_Depth( Depth );
		return pFX;
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'R': Set_Rate( v );		break;
			case 'D': Set_Depth( v );		break;
			case 'M': Set_Mode( v ); 		break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Rate(R)   : " );	println( Rate );
		print( "Depth(D)  : " );	println( Depth );
		print( "Mode(M1/2): " );	println( Mode );
	}

	virtual void Print_Caption()
	{
		print( "Tremolo" );
	}
};

#endif
