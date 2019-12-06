#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "Audio.h"
#include "Myutil.h"
#include "vcp_mini.h"
#include "Menu.h"
#include "FX_Process.h"
#include "Line_Editor.h"

void Print_FX_Caption()
{
	Print_FX_List();
	print( "#" );	print( Get_Current_Index() ); print( "." );
	Menu->Print_Caption();
	println( "" );
	Menu->Print_Paramaters();
}

void Print_Usage()
{
	println( "エフェクトを変更するには#の後に0から10までの数字を入力してリターンキーを押してください。" );
	println( "To change the FX, enter a number from 0 to 10 after the # and press the return key. ex) #1 (enter)");
	println( "パラメーターを変更するにはアルファベット1文字の後に0から99までの数字を入力してリターンキーを押してください。" );
	println( "To change the FX parameter, enter a number from 0 to 99 after one alphabet and press the return key. ex) t99 (enter)");
	println( "" );

	Print_FX_Caption();
}

void Get_Terminal_Value()
{
	int		v = 0;
  char	str[ 5 ];

  Line_Editor( str );

  char c = toupper( str[ 0 ] );

  v = atoi( str+1 );
	v = Limit( 0, v, 99 );

	if( c == '#' )	Select_Menu( v );
	else						Menu->Set_Param( c, v );

	Print_FX_Caption();
}
