/*
 * Menu_Flanger.h
 *
 *  Created on: Jun 26, 2019
 *      Author: gamenyge
 */

#ifndef MENU_Flanger_H_
#define MENU_Flanger_H_

#include	"Menu_Interface.h"
#include	"..\FX_Process.h"
#include	"Common.h"

struct Menu_Flanger : public Menu_Interface
{
	int	Rate, Depth, Mix_Level, Time, Feedback;

	Menu_Flanger():
		Rate( 30 ), Depth( 30 ), Mix_Level( 70 ), Time( 20 ), Feedback( 70 )
	{
	}

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Flanger; }

	void Set_Rate( int v )
	{
		Rate = v;
		FX->Set_Control_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Depth( int v )
	{
		Depth = v;
		FX->Set_Control_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Time( int v )
	{
		Time = v;
		FX->Set_Control_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Feedback( int v )
	{
		Feedback = v;
		FX->Set_Control_3( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Control_4( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Flanger );

		Set_Rate( Rate );
		Set_Depth( Depth );
		Set_Time( Time );
		Set_Feedback( Feedback );
		Set_Mix_Level( Mix_Level );
	}

	virtual void Set_Control( char c, int v )
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
		printf( "Rate(R)    : %d\n", Rate );
		printf( "Depth(D)   : %d\n", Depth );
		printf( "Time(D)    : %d\n", Time );
		printf( "Feedback(D): %d\n", Feedback );
		printf( "Mix(M)     : %d\n", Mix_Level );
	}

	virtual void Print_Caption()
	{
		printf( "Flanger" );
	}
};

#endif /* MENU_Flanger_H_ */
