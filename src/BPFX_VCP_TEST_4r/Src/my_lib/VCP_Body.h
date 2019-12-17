extern "C" int _read(int file, char *ptr, int len )
{
	char c;

	if( HAL_UART_Receive( &HUART, (uint8_t*)&c, 1, 0xFFFF ) == HAL_OK )
	{
		HAL_UART_Transmit( &HUART, (uint8_t*)&c, 1, 0xFFFF ); // echo back
	  *ptr = c;
	  return 1;
	}
	return 0;
}

extern "C" int _write( int file, char *ptr, int len )
{
	if( HAL_UART_Transmit( &HUART, (uint8_t*)ptr, len, 0xFFFF ) == HAL_OK )
		return len;
	return 0;
}

static void Print_Unique_ID()
{
	uint32_t	Unique_ID[ 3 ];

#if	defined(STM32F103xB)
	Unique_ID[ 0 ] = *( ( uint32_t* )0x1FFFF7E8 );
	Unique_ID[ 1 ] = *( ( uint32_t* )0x1FFFF7EC );
	Unique_ID[ 2 ] = *( ( uint32_t* )0x1FFFF7F0 );
#endif
#if	defined(STM32L152xE)
	Unique_ID[ 0 ] = *( ( uint32_t* )0x1FF800D0 );
	Unique_ID[ 1 ] = *( ( uint32_t* )0x1FF800D4 );
	Unique_ID[ 2 ] = *( ( uint32_t* )0x1FF800D8 );
#endif

	printf( "Unique ID\t: %08lX-%08lX-%08lX",	Unique_ID[ 0 ], Unique_ID[ 1 ],	Unique_ID[ 2 ] );
}

static void Print_Flash_Size()
{
#if	defined(STM32F103xB)
	uint16_t	size = *( ( uint16_t* )0x1FFFF7E0 );
#endif
#if	defined(STM32L152xE)
	uint16_t	size = *( ( uint16_t* )0x1FF800CC );
#endif

	printf( "Flash Size\t: %u Kbytes", size );
}

static void Print_System_Clock()
{
	printf( "System Clock\t: %lu MHz", SystemCoreClock / 1000 / 1000 );
}

static void Print_Device_ID()
{
	printf( "Device ID\t: %08lx", HAL_GetDEVID() );
}

static void Print_Option_Bytes()
{
	int i;
	printf( "Option Bytes\t: " );

#if	defined(STM32F103xB)
	for( i = 0; i < 16; ++i )
	{
		uint8_t	v = *( uint8_t* )( 0x1FFFF800+i );
		if( i == 4 || i == 6 )	printf( "[%02X] ", v );
		else										printf( "%02X ", v );
	}
#endif
#if	defined(STM32L152xE)
	for( i = 0; i < 8; ++i )	// アドレスと構成が異なる
	{
		uint8_t	v = *( uint8_t* )( 0x1FF80000+i );
		if( i == 5 || i == 7 )	printf( "[%02X] ", v );
		else										printf( "%02X ", v );
	}
#endif
}

void Welcome( const char* str )
{
	printf( "\r\n>>> Welcome to STM32%s <<<\r\n", str );
	Print_System_Clock();	puts( "\r" );
	Print_Flash_Size();		puts( "\r" );
	Print_Device_ID();		puts( "\r" );
	Print_Unique_ID();		puts( "\r" );
	Print_Option_Bytes();	puts( "\r" );
}
