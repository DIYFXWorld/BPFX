#include "Menu.h"
#include "Menu_Item_FX_None.h"
#include "Menu_Item_FX_Delay_0.h"
#include "Menu_Item_FX_Delay.h"
#include "Menu_Item_FX_Analog_Delay.h"
#include "Menu_Item_FX_Delay_PCMU.h"
#include "Menu_Item_FX_Delay_ADPCM.h"
#include "Menu_Item_FX_Two_Tap_Delay.h"
#include "Menu_Item_FX_Reverse_Delay.h"
#include "Menu_Item_FX_Reverse_Delay_PCMU.h"
#include "Menu_Item_FX_Modulation_Delay.h"

FX_Menu	Menu( 9 );

Menu_Item_FX_None								_Menu_Item_FX_None;
Menu_Item_FX_Delay_0						_Menu_Item_FX_Delay_0;
Menu_Item_FX_Delay							_Menu_Item_FX_Delay;
Menu_Item_FX_Analog_Delay				_Menu_Item_FX_Analog_Delay;
Menu_Item_FX_Delay_PCMU					_Menu_Item_FX_Delay_PCMU;
Menu_Item_FX_Delay_ADPCM				_Menu_Item_FX_Delay_ADPCM;
Menu_Item_FX_Two_Tap_Delay			_Menu_Item_FX_Two_Tap_Delay;
Menu_Item_FX_Reverse_Delay			_Menu_Item_FX_Reverse_Delay;
Menu_Item_FX_Reverse_Delay_PCMU	_Menu_Item_FX_Reverse_Delay_PCMU;
Menu_Item_FX_Modulation_Delay		_Menu_Item_FX_Modulation_Delay;

void Menu_Initial()
{
	Menu.Push_Back( &_Menu_Item_FX_None );
	Menu.Push_Back( &_Menu_Item_FX_Delay_0 );
	Menu.Push_Back( &_Menu_Item_FX_Delay );
	Menu.Push_Back( &_Menu_Item_FX_Analog_Delay );
	Menu.Push_Back( &_Menu_Item_FX_Delay_PCMU );
	Menu.Push_Back( &_Menu_Item_FX_Delay_ADPCM );
	Menu.Push_Back( &_Menu_Item_FX_Two_Tap_Delay );
	Menu.Push_Back( &_Menu_Item_FX_Reverse_Delay );
	Menu.Push_Back( &_Menu_Item_FX_Reverse_Delay_PCMU );
	Menu.Push_Back( &_Menu_Item_FX_Modulation_Delay );
}
