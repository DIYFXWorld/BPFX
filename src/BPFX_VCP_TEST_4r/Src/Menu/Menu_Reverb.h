#ifndef MENU_Reverb_H_
#define MENU_Reverb_H_

#include	"Menu_Interface.h"
#include	"FX_Process.h"
#include	"Common.h"

struct Menu_Reverb : public Menu_Interface
{
	int	Room_Size, Mix_Level;

	Menu_Reverb():
		Room_Size( 80 ), Mix_Level( 60 )
	{
	}

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Reverb; }

	void Set_Room_Size( int v )
	{
		Room_Size = v;
		FX->Set_Control_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Control_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Reverb );
		Set_Room_Size( Room_Size );
		Set_Mix_Level( Mix_Level );
		FX->Reset = true;
	}

	virtual void Set_Control( char c, int v )
	{
		switch( c )
		{
			case 'R': Set_Room_Size( v ); 			break;
			case 'M': Set_Mix_Level( v );	break;
		}
	}

	virtual void Print_Paramaters()
	{
		printf( "Room Size(R): %d\n", Room_Size );
		printf( "Mix(M)      : %d\n", Mix_Level );
	}

	virtual void Print_Caption()
	{
		printf( "Reverb" );
	}
};

#endif /* MENU_Reverb_H_ */
