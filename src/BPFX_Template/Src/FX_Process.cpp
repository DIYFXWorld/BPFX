#include <Audio.h>
#include <hal.h>

extern int Param_0, Param_1, Param_2;

void Audio_Input_Callback( int input )
{
	Audio::Set_Output( input );
}
