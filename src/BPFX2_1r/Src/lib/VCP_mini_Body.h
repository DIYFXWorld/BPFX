const int STRING_BUFFER_SIZE = 21;

/*
static void READ( char *ptr )
{
	char c;

	if( HAL_UART_Receive( &HANDLE, (uint8_t*)&c, 1, 0xFFFF ) == HAL_OK )
	{
		HAL_UART_Transmit( &HANDLE, (uint8_t*)&c, 1, 0xFFFF ); // echo back
	  *ptr = c;
	}
}
*/

static void WRITE( const char *ptr, int len )
{
	HAL_UART_Transmit( &HANDLE, (uint8_t*)ptr, len, 0xFFFF );
}

void print( const char v ){ WRITE( ( char* )&v, 1 ); }

void print( const unsigned char v ){ WRITE( ( char* )&v, 1 ); }

void print( const char* v ){ WRITE( ( char* )v, strlen( v ) ); }

void print( const unsigned char* v ){ WRITE( ( char* )v, strlen( ( const char* )v ) ); }

void print( int v, int radix )
{
	char string_buffer[ STRING_BUFFER_SIZE ];
	itoa( v, string_buffer, radix );
	WRITE( string_buffer, strlen( string_buffer ) );
}

void print( uint16_t v, int radix )
{
	char string_buffer[ STRING_BUFFER_SIZE ];
	utoa( v, string_buffer, radix );
	WRITE( string_buffer, strlen( string_buffer ) );
}

void print( uint32_t v, int radix )
{
	char string_buffer[ STRING_BUFFER_SIZE ];
	utoa( v, string_buffer, radix );
	WRITE( string_buffer, strlen( string_buffer ) );
}

void Print_Flash_Size()
{
	uint16_t	size = *( ( uint16_t* )0x1FFFF7CC );
	print( "Flash Size : " );
	print( size, DEC );
	print( " Kbytes" );
}

void Print_System_Clock()
{
	print( "System Clock : " );
	print( SystemCoreClock / 1000 / 1000, DEC );
	print( " MHz" );
}

void Print_Device_ID()
{
	print( "Device ID : " );
	print( HAL_GetDEVID(), HEX );
}

void Welcome( const char* str )
{
	print( "\r\n" );
	print( ">>> Welcome to STM32" );
	print( str );
	print( " <<<\r\n"  );

	Print_Flash_Size();	print( "\r\n" );
	Print_System_Clock();	print( "\r\n" );
	Print_Device_ID();	print( "\r\n" );
}

////////

void println( char v )
{
	print( v );
	print( '\n' );
}

void println( unsigned char v )
{
	print( v );
	print( '\n' );
}


void println( const char* v )
{
	print( v );
	print( '\n' );
}

void println( const unsigned char* v )
{
	print( v );
	print( '\n' );
}
void println( int v, int r )
{
	print( v, r );
	print( '\n' );
}

void println( uint16_t v, int r )
{
	print( v, r );
	print( '\n' );
}

void println( uint32_t v , int r )
{
	print( v, r );
	print( '\n' );
}
