#include	"hal.h"
#include	"Line_Editor.h"
#include	<ctype.h>

#define	HUART							huart1
extern UART_HandleTypeDef HUART;

#define ASCII_BS   	0x08
#define ASCII_LF   	0x0A
#define ASCII_CR   	0x0D
#define ASCII_SPACE	0x20

const uint8_t	RETURN[ 2 ] = { ASCII_CR, ASCII_LF };
const	uint8_t BS[ 3 ] 		= { ASCII_BS, ASCII_SPACE, ASCII_BS };

void Line_Editor( char* buffer )
{
	const char *top = buffer;
	uint8_t c;

	while( 1 )
	{
		if( HAL_UART_Receive( &HUART, &c, 1, 0xFFFF ) == HAL_OK )
		{
			if( isgraph( c ) || c == ASCII_SPACE )
			{
				HAL_UART_Transmit( &HUART, &c, 1, 0xFFFF );
				*buffer = c;
				++buffer;
			}
			else
			{
				if( c == ASCII_CR )
				{
					HAL_UART_Transmit( &HUART, (uint8_t*)RETURN, 2, 0xFFFF );
					*buffer = 0;
					return;
				}
				else if( c == ASCII_BS )
				{
					if( top < buffer )
					{
						HAL_UART_Transmit( &HUART, (uint8_t*)BS, 3, 0xFFFF );
						--buffer;
						*buffer = 0;
					}
				}
			}
		}
	}
}

// example line_editor
// get number from terminal
void get_int( char* msg, int* v )
{
  char buffer[ 20 ];
  puts( msg );
  Line_Editor( buffer );
  sscanf( buffer, "%d", v );
  puts( "" );
}

