#ifndef MENU_Pitch_Shift_H_
#define MENU_Pitch_Shift_H_

#include	"..\FX_Process.h"
#include	"Myutil.h"
#include	"Menu_Interface.h"

struct Menu_Pitch_Shift : public Menu_Interface
{
	int8_t	Pitch, Fine, Mix_Level;

	Menu_Pitch_Shift():
		Pitch( 50 ), Fine( 50 ), Mix_Level( 99 )
	{
	}

	void Set_Pitch( int v )
	{
		v = Limit<37,63>( v );
		Pitch = v;
		FX->Set_Param_0( Map( v, 37, 63, 0, 4095 ) );
	}

	void Set_Fine( int v )
	{
		Fine = v;
		FX->Set_Param_1( Map( v, 0, 99, 0, 4095 ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Pitch_Shift );
		Set_Pitch( Pitch );
		Set_Fine( Fine );
		Set_Mix_Level( Mix_Level );
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'T': Set_Pitch( v ); 		break;
			case 'F': Set_Fine( v );			break;
			case 'M': Set_Mix_Level( v );	break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Pitch(P): " );	println( Pitch );
		print( "Fine(F) : " );	println( Fine );
		print( "Mix(M)  : " );	println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Pitch Shift" );
	}
};

#endif /* MENU_DELAY_H_ */
