#include <stdio.h>
#include <ctype.h>
#include <Menu.h>
#include <stdlib.h>
#include "Audio.h"
#include "Myutil.h"
#include "vcp_mini.h"
#include "Line_Editor.h"
#include "FX_Terminal.h"
#include "main_cpp.h"

void Print_Usage()
{
	println( "エフェクトを変更するには#の後に0から10までの数字を入力してリターンキーを押してください。" );
	println( "To change the FX, enter a number from 0 to 10 after the # and press the return key. ex) #1 (enter)");
	println( "パラメーターを変更するにはアルファベット1文字の後に0から99までの数字を入力してリターンキーを押してください。" );
	println( "To change the FX parameter, enter a number from 0 to 99 after one alphabet and press the return key. ex) t99 (enter)");
	println( "" );

	Menu.Print_FX_Caption();
}

void Get_Terminal_String()
{
	int		v = 0;
  char	str[ 5 ];

  Line_Editor( str );

  char c = toupper( str[ 0 ] );

  v = atoi( str+1 );
	v = Limit( 0, v, 99 );

	if( c == '#' )
	{
	  FX_Interface* fx = Menu.Select( v );
	  if( fx )	FX = fx;
	}
	else						Menu.Set_Param( c, v );

	Menu.Print_FX_Caption();
}
