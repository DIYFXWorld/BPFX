/*
 * Menu.cpp
 *
 *  Created on: 2019/06/24
 *      Author: gamenyge
 */
#include	"Menu.h"
#include	<stdio.h>
#include	<list>

#include	"Menu_FX_None.h"
#include	"Menu_Delay.h"
#include	"Menu_Analog_Delay.h"
#include	"Menu_Reverse_Delay.h"
#include	"Menu_Modulation_Delay.h"
#include	"Menu_Chorus.h"
#include	"Menu_Flanger.h"
#include	"Menu_Phaser.h"
#include	"Menu_Tremolo.h"
#include	"Menu_Ring_Modulation.h"
#include	"Menu_Reverb.h"

typedef	std::list<Menu_Interface*>	Type_Menus;
Type_Menus													Menus;
Menu_Interface											*Menu;

void Menu_Initial()
{
	Menus.push_back( new Menu_FX_None );
	Menus.push_back( new Menu_Delay );
	Menus.push_back( new Menu_Analog_Delay );
	Menus.push_back( new Menu_Reverse_Delay );
	Menus.push_back( new Menu_Modulation_Delay );
	Menus.push_back( new Menu_Chorus );
	Menus.push_back( new Menu_Flanger );
	Menus.push_back( new Menu_Phaser );
	Menus.push_back( new Menu_Tremolo );
	Menus.push_back( new Menu_Ring_Modulation );
	Menus.push_back( new Menu_Reverb );
}

void Select_Menu( int index )
{
	auto it = std::next( Menus.begin(), index );

	if( it != Menus.end() )
	{
		Menu = *it;
		Select_FX( Menu->Get_ID() );
		Menu->Active();
	}
}

void Print_FX_List()
{
	int	i = 0;
	int	count = 0;

	for( auto it( Menus.begin() ); it != Menus.end(); ++it )
	{
		printf( "#%d.", i );
		( *it )->Print_Caption();
		printf( "   " );
		++count;
		if( count == 4 )
		{
			printf( "\n" );
			count = 0;
		}
		++i;
	}
	printf( "\n" );
	if( count % 4 )	printf( "\n" );
}

int Get_Current_Index()
{
	int	i = 0;

	for( auto it( Menus.begin() ); it != Menus.end(); ++it )
	{
		if( *it == Menu )	return i;
		++i;
	}

	return -1;	// error
}
