#ifndef MENU_Reverb_H_
#define MENU_Reverb_H_

#include	"..\FX_Process.h"
#include	"Myutil.h"
#include	"Menu_Interface.h"

struct Menu_Reverb : public Menu_Interface
{
	int8_t	Room_Size, Mix_Level;

	Menu_Reverb():
		Room_Size( 80 ), Mix_Level( 60 )
	{
	}

	void Set_Room_Size( int v )
	{
		Room_Size = v;
		FX->Set_Param_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Param_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Reverb );
		Set_Room_Size( Room_Size );
		Set_Mix_Level( Mix_Level );
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'R': Set_Room_Size( v ); 			break;
			case 'M': Set_Mix_Level( v );	break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Room Size(R): " );	println( Room_Size );
		print( "Mix(M)      : " );	println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Reverb" );
	}
};

#endif /* MENU_Reverb_H_ */
