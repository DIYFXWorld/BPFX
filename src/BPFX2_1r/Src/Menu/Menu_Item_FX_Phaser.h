#ifndef Menu_Item_FX_Phaser_h_
#define Menu_Item_FX_Phaser_h_

#include	"Myutil.h"
#include	"Menu_Item_Interface.h"
#include	"FX_Phaser.h"

struct Menu_Item_FX_Phaser : public Menu_Item_Interface
{
	int8_t	Rate, Depth, Feedback, Mix_Level, APF_of_Numbers;

	Menu_Item_FX_Phaser():
		Rate( 50 ), Depth( 50 ), Feedback( 20 ), Mix_Level( 99 ),
		APF_of_Numbers( 6 )
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

	void Set_Feedback( int v )
	{
		Feedback = v;
		pFX->Set_Param_3( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_APF_of_Numbers( int v )
	{
		if( v != 2 && v != 4 && v != 6 )	v = 6;
		APF_of_Numbers = v;
		pFX->Set_Param_4( v );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		pFX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_Phaser;
		Set_Rate( Rate );
		Set_Depth( Depth );
		Set_Feedback( Feedback );
		Set_APF_of_Numbers( APF_of_Numbers );
		Set_Mix_Level( Mix_Level );
		return pFX;
	}

	virtual void Set_Param( char c, int v )
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
		print( "Rate(R)               : " );	println( Rate );
		print( "Depth(D)              : " );	println( Depth );
		print( "Feedback(F)           : " );	println( Feedback );
		print( "APF of Numbers(A2/4/6): " );	println( APF_of_Numbers );
		print( "Mix(M)                : " );	println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Phaser" );
	}
};

#endif
