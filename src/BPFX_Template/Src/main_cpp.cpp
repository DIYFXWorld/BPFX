#include <lib.h>
#include <stdlib.h>

int	Param_0 = 2048, Param_1 = 2048, Param_2 = 2048;

extern "C" void main_cpp()
{
//	puts( "Blue Pill Sound FX Projects - VCP Test Template\n" );

	Audio::Start();
	Audio::Active();

	char	str[ 5 ];

	while( 1 )
	{
		gets( str );

		switch( str[ 0 ] )
		{
			case 'a':	case 'A':	Param_0 = atoi( str+1 ); break;
			case 'b':	case 'B': Param_1 = atoi( str+1 ); break;
			case 'c':	case 'C': Param_2 = atoi( str+1 ); break;
		}
	}
}
