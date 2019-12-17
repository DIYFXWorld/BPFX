/*
 * Terminal_Control.cpp
 *
 *  Created on: 2019/01/26
 *      Author: gamenyge
 */

#include	<stdio.h>
#include	<ctype.h>
#include	"Line_Editor.h"
#include	"Audio.h"
#include	"Common.h"
#include	"FX_Process.h"
#include	"Menu.h"

void Print_FX_Caption()
{
	Print_FX_List();
	printf( "#%d.", Get_Current_Index() );
	Menu->Print_Caption();
	printf( "\n" );
	Menu->Print_Paramaters();
}

void Print_Usage()
{
	puts( "エフェクトを変更するには#の後に0から7までの数字を入力してリターンキーを押してください。\r" );
	puts( "To change the FX, enter a number from 0 to 7 after the # and press the return key. ex) #1 (enter)");
	puts( "\r" );
	puts( "パラメーターを変更するにはアルファベット1文字の後に0から99までの数字を入力してリターンキーを押してください。\r" );
	puts( "To change the FX parameter, enter a number from 0 to 99 after one alphabet and press the return key. ex) t99 (enter)");
	puts( "\r" );

	Print_FX_Caption();
}

void Get_Terminal_Value()
{
	int		v = 0;
  char	str[ 5 ];

  Line_Editor( str );

  char c = toupper( str[ 0 ] );

  sscanf( str+1, "%d", &v );

	v = Limit( 0, v, 99 );

	if( c == '#' )	Select_Menu( v );
	else						Menu->Set_Control( c, v );

	Print_FX_Caption();
}
