#include "Menu.h"
#include "Menu_Item_FX_None.h"
#include "Menu_Item_FX_Chorus_0.h"
#include "Menu_Item_FX_Chorus.h"
#include "Menu_Item_FX_Dual_Chorus.h"
#include "Menu_Item_FX_String_Chorus.h"
#include "Menu_Item_FX_Flanger_0.h"
#include "Menu_Item_FX_Flanger.h"
#include "Menu_Item_FX_Phaser.h"
#include "Menu_Item_FX_Reverb.h"
#include "Menu_Item_FX_Tremolo.h"
#include "Menu_Item_FX_Ring_Modulation.h"

FX_Menu	Menu( 11 );

Menu_Item_FX_None							_Menu_Item_FX_None;
Menu_Item_FX_Chorus_0					_Menu_Item_FX_Chorus_0;
Menu_Item_FX_Chorus						_Menu_Item_FX_Chorus;
Menu_Item_FX_Dual_Chorus			_Menu_Item_FX_Dual_Chorus;
Menu_Item_FX_String_Chorus		_Menu_Item_FX_String_Chorus;
Menu_Item_FX_Flanger_0				_Menu_Item_FX_Flanger_0;
Menu_Item_FX_Flanger					_Menu_Item_FX_Flanger;
Menu_Item_FX_Phaser						_Menu_Item_FX_Phaser;
Menu_Item_FX_Reverb						_Menu_Item_FX_Reverb;
Menu_Item_FX_Tremolo					_Menu_Item_FX_Tremolo;
Menu_Item_FX_Ring_Modulation	_Menu_Item_FX_Ring_Modulation;

void Menu_Initial()
{
	Menu.Push_Back( &_Menu_Item_FX_None );
	Menu.Push_Back( &_Menu_Item_FX_Chorus_0 );
	Menu.Push_Back( &_Menu_Item_FX_Chorus );
	Menu.Push_Back( &_Menu_Item_FX_Dual_Chorus );
	Menu.Push_Back( &_Menu_Item_FX_String_Chorus );
	Menu.Push_Back( &_Menu_Item_FX_Flanger_0 );
	Menu.Push_Back( &_Menu_Item_FX_Flanger );
	Menu.Push_Back( &_Menu_Item_FX_Phaser );
	Menu.Push_Back( &_Menu_Item_FX_Reverb );
	Menu.Push_Back( &_Menu_Item_FX_Tremolo );
	Menu.Push_Back( &_Menu_Item_FX_Ring_Modulation );
}
