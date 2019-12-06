#include	"Audio.h"
#include	"Menu.h"
#include	<stdio.h>
#include	<list>

#include	"Menu_Delay_0.h"
#include	"Menu_FX_None.h"
#include	"Menu_Delay.h"
#include	"Menu_Analog_Delay.h"
#include	"Menu_Reverse_Delay.h"
#include	"Menu_Modulation_Delay.h"
#include	"Menu_Chorus.h"
#include	"Menu_Flanger.h"
#include	"Menu_Phaser.h"
#include	"Menu_Reverb.h"
#include	"Menu_Two_Tap_Delay.h"
#include	"Menu_Delay_ADPCM.h"
#include	"Menu_Delay_PCMU.h"
#include	"Menu_Reverse_Delay_PCMU.h"
#include	"Menu_Pitch_Shift.h"

typedef	std::list<Menu_Interface*>	Type_Menus;
Type_Menus			Menus;
Menu_Interface	*Menu;

void Menu_Initial()
{
	Menus.push_back( new Menu_FX_None );
	Menus.push_back( new Menu_Delay_0 );
	Menus.push_back( new Menu_Delay );
	Menus.push_back( new Menu_Analog_Delay );
	Menus.push_back( new Menu_Delay_PCMU );
	Menus.push_back( new Menu_Delay_ADPCM );
	Menus.push_back( new Menu_Two_Tap_Delay );
	Menus.push_back( new Menu_Reverse_Delay );
	Menus.push_back( new Menu_Reverse_Delay_PCMU );
	Menus.push_back( new Menu_Modulation_Delay );
	Menus.push_back( new Menu_Chorus );
	Menus.push_back( new Menu_Flanger );
	Menus.push_back( new Menu_Phaser );
	Menus.push_back( new Menu_Reverb );
}

void Select_Menu( int index )
{
	auto it = std::next( Menus.begin(), index );

	if( it != Menus.end() )
	{
		Audio::Inactive();
		Menu = *it;
		Menu->Active();
		FX->Reset();
		Audio::Active();
	}
}

void Print_FX_List()
{
	int	i = 0;
	int	count = 0;

	for( auto it : Menus )
	{
		print( "#" ); print( i ); print( "." );

		it->Print_Caption();
		print( "   " );
		++count;
		if( count == 3 )
		{
			println( "" );
			count = 0;
		}
		++i;
	}
	println( "" );
	if( count % 3 )	println( "" );
}

int Get_Current_Index()
{
	int	i = 0;

	for( auto it : Menus )
	{
		if( it == Menu )	return i;
		++i;
	}

	return -1;	// error
}
