#ifndef MENU_Tremolo_H_
#define MENU_Tremolo_H_

#include	"Menu_Interface.h"
#include	"..\FX_Process.h"
#include	"Common.h"

struct Menu_Tremolo : public Menu_Interface
{
	int	Rate, Depth, Mode;

	Menu_Tremolo():
		Rate( 50 ), Depth( 80 ), Mode( 1 )
	{
	}

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Tremolo; }

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

	void Set_Mode( int v )
	{
		if( v == 1 || v == 2 )
		Mode = v;
		FX->Set_Control_4( v );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Tremolo );
		Set_Rate( Rate );
		Set_Depth( Depth );
	}

	virtual void Set_Control( char c, int v )
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
		printf( "Rate(R)   : %d\n", Rate );
		printf( "Depth(D)  : %d\n", Depth );
		printf( "Mode(M1/2): %d\n", Mode );
	}

	virtual void Print_Caption()
	{
		printf( "Tremolo" );
	}
};

#endif /* MENU_Tremolo_H_ */
