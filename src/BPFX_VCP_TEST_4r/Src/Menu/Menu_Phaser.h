/*
 * Menu_Phaser.h
 *
 *  Created on: Jun 26, 2019
 *      Author: gamenyge
 */

#ifndef MENU_Phaser_H_
#define MENU_Phaser_H_

#include	"Menu_Interface.h"
#include	"..\FX_Process.h"
#include	"Common.h"

struct Menu_Phaser : public Menu_Interface
{
	int	Rate, Depth, Feedback, Mix_Level, APF_of_Numbers;

	Menu_Phaser():
		Rate( 50 ), Depth( 50 ), Feedback( 20 ), Mix_Level( 99 ),
		APF_of_Numbers( 6 )
	{
	}

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Phaser; }

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

	void Set_Feedback( int v )
	{
		Feedback = v;
		FX->Set_Control_3( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_APF_of_Numbers( int v )
	{
		if( v != 2 && v != 4 && v != 6 )	v = 6;
		APF_of_Numbers = v;
		FX->Set_Control_4( v );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Control_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Phaser );
		Set_Rate( Rate );
		Set_Depth( Depth );
		Set_Feedback( Feedback );
		Set_APF_of_Numbers( APF_of_Numbers );
		Set_Mix_Level( Mix_Level );
	}

	virtual void Set_Control( char c, int v )
	{
		switch( c )
		{
			case 'R': Set_Rate( v );		 				break;
			case 'D': Set_Depth( v ); 					break;
			case 'F': Set_Feedback( v );				break;
			case 'A': Set_APF_of_Numbers( v );	break;
			case 'M': Set_Mix_Level( v );				break;
		}
	}

	virtual void Print_Paramaters()
	{
		printf( "Rate(R)               : %d\n", Rate );
		printf( "Depth(D)              : %d\n", Depth );
		printf( "Feedback(F)           : %d\n", Feedback );
		printf( "APF of Numbers(A2/4/6): %d\n", APF_of_Numbers );
		printf( "Mix(M)                : %d\n", Mix_Level );
	}

	virtual void Print_Caption()
	{
		printf( "Phaser" );
	}
};

#endif /* MENU_Phaser_H_ */
