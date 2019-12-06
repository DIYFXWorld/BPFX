#ifndef MENU_DELAY_ADPCM_H_
#define MENU_DELAY_ADPCM_H_

#include	"..\FX_Process.h"
#include	"Myutil.h"
#include	"Menu_Interface.h"

struct Menu_Delay_ADPCM : public Menu_Interface
{
	int8_t	Time, Feedback, Mix_Level;

	Menu_Delay_ADPCM():
		Time( 99 ), Feedback( 0 ), Mix_Level( 99 )
	{
	}

	void Set_Time( int v )
	{
		Time = v;
		FX->Set_Param_2( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Feedback( int v )
	{
		Feedback = v;
		FX->Set_Param_0( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	void Set_Mix_Level( int v )
	{
		Mix_Level = v;
		FX->Set_Param_1( Map( v, 0, 99, 0, UINT12_MAX ) );
	}

	virtual void Active()
	{
		Select_FX( FX_ID_Delay_ADPCM );
		Set_Time( Time );
		Set_Feedback( Feedback );
		Set_Mix_Level( Mix_Level );
	}

	virtual void Set_Param( char c, int v )
	{
		switch( c )
		{
			case 'T': Set_Time( v ); 			break;
			case 'F': Set_Feedback( v );	break;
			case 'M': Set_Mix_Level( v );	break;
		}
	}

	virtual void Print_Paramaters()
	{
		print( "Time(T)    : " );	println( Time );
		print( "Feedback(F): " );	println( Feedback );
		print( "Mix(M)     : " );	println( Mix_Level );
	}

	virtual void Print_Caption()
	{
		print( "Delay(ADPCM/fx10kHz)" );
	}
};

#endif /* MENU_DELAY_H_ */
