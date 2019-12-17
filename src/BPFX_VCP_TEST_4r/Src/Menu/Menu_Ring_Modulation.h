/*
 * Menu_Ring_Modulation.h
 *
 *  Created on: Jun 26, 2019
 *      Author: gamenyge
 */

#ifndef MENU_Ring_Modulation_H_
#define MENU_Ring_Modulation_H_

#include	"Menu_Interface.h"
#include	"..\FX_Process.h"
#include	"Common.h"

struct Menu_Ring_Modulation : public Menu_Interface
{
	int	Rate;

	Menu_Ring_Modulation():
		Rate( 50 )
	{
	}

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Ring_Modulation; }

	void Set_Rate( int v )
	{
		Rate = v;
		FX->Set_Control_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Ring_Modulation );
		Set_Rate( Rate );
	}

	virtual void Set_Control( char c, int v )
	{
		switch( c )
		{
			case 'R': Set_Rate( v );		break;
		}
	}

	virtual void Print_Paramaters()
	{
		printf( "Rate(R) : %d\n", Rate );
	}

	virtual void Print_Caption()
	{
		printf( "Ring Modulation" );
	}
};

#endif /* MENU_Ring_Modulation_H_ */
