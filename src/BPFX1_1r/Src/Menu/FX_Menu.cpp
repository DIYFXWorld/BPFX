#include	"FX_Menu.h"
#include	"Audio.h"
#include	"vcp_mini.h"

FX_Menu::FX_Menu( int length ):
	Items( length ),
	Count( 0 ), 
	Index( -1 )
{
}

void FX_Menu::Push_Back( Menu_Item_Interface* v )
{
	Items[ Count ] = v;
	++Count;
}

FX_Interface* FX_Menu::Select( int idx )
{
	if( ( idx >= 0 ) && ( idx < Count ) )
	{
		Audio::Inactive();

		if( 0 <= Index )	Items[ Index ]->Destroy_FX();

		Index = idx;

		FX_Interface *fx = Items[ Index ]->Create_FX();

		fx->Reset();

		Audio::Active();

		return fx;
	}
	return 0;
}

void FX_Menu::Set_Param( char c, int v )
{
	if( ( Index >= 0 ) && ( Index < Count ) && ( &Items[ Index ] ) )
	{
		Items[ Index ]->Set_Param( c, v );
	}
}

void FX_Menu::Print_FX_List()
{
	int	c = 0;

	for( auto i( 0 ); i < Count; ++i )
	{
		print( "#" ); print( i ); print( "." );

		Items[ i ]->Print_Caption();
		print( "   " );
		++c;

		if( c == 3 )
		{
			println( "" );
			c = 0;
		}
	}
	println( "" );
	if( c % 3 )	println( "" );
}

void FX_Menu::Print_FX_Caption()
{
	Print_FX_List();
	print( "#" ); print( Index ); print( "." );
	Items[ Index ]->Print_Caption();
	println( "" );
	Items[ Index ]->Print_Paramaters();
}
