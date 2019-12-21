#ifndef Menu_Item_FX_Ring_Modulation_h_
#define Menu_Item_FX_Ring_Modulation_h_

#include "Myutil.h"
#include "Menu_Item_Interface.h"
#include "FX_Ring_Modulation.h"

struct Menu_Item_FX_Ring_Modulation : public Menu_Item_Interface
{
	int16_t	Frequency;

	Menu_Item_FX_Ring_Modulation():
		Frequency( 50 )
	{
	}

	void Set_Frequency( int v )
	{
		Frequency = v;
		pFX->Set_Param_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_Ring_Modulation;
		Set_Frequency( Frequency );
		return pFX;
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'F': Set_Frequency( v );		break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Frequency(F) : " );	println( Frequency );
	}

	virtual void Print_Caption()
	{
		print( "Ring Modulation" );
	}
};

#endif
