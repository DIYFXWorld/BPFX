#ifndef MENU_Chorus_H_
#define MENU_Chorus_H_

#include	"Menu_Interface.h"
#include	"..\FX_Process.h"
#include	"Common.h"

struct Menu_Chorus : public Menu_Interface
{
	int	Rate, Depth, Mix_Level;

	Menu_Chorus():
		Rate( 30 ), Depth( 30 ), Mix_Level( 70 )
	{
	}

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Chorus; }

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

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Control_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Chorus );
		Set_Rate( Rate );
		Set_Depth( Depth );
		Set_Mix_Level( Mix_Level );
	}

	virtual void Set_Control( char c, int v )
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
		printf( "Rate(R) : %d\n", Rate );
		printf( "Depth(D): %d\n", Depth );
		printf( "Mix(M)  : %d\n", Mix_Level );
	}

	virtual void Print_Caption()
	{
		printf( "Chorus" );
	}
};

#endif /* MENU_Chorus_H_ */
