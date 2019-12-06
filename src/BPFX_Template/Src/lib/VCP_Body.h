extern "C" int _read(int file, char *ptr, int len )
{
	char c;

	if( HAL_UART_Receive( &HVCP, (uint8_t*)&c, 1, 0xFFFF ) == HAL_OK )
	{
//		HAL_UART_Transmit( &HVCP, (uint8_t*)&c, 1, 0xFFFF ); // echo back
	  *ptr = c;
	  return 1;
	}
	return 0;
}

extern "C" int _write( int file, char *ptr, int len )
{
	if( HAL_UART_Transmit( &HVCP, (uint8_t*)ptr, len, 0xFFFF ) == HAL_OK )
		return len;
	return 0;
}

static void Print_Unique_ID()
{
#ifdef	UID_BASE
	printf( "Unique ID    : %08lX-%08lX-%08lX",
		HAL_GetUIDw0(), HAL_GetUIDw1(),	HAL_GetUIDw2() );
#else
	printf( "Unique ID    : No Suport" );
#endif
}

static void Print_Flash_Size()
{
#if	defined(STM32F103xB)
	uint16_t	size = *( ( uint16_t* )0x1FFFF7E0 );
#endif
#if	defined(STM32L152xE)
	uint16_t	size = *( ( uint16_t* )0x1FF800CC );
#endif
#if	defined(STM32F411xE)
	uint16_t	size = *( ( uint16_t* )0x1FFF7A22 );
#endif
#if	defined(STM32F303xC)
	uint16_t	size = *( ( uint16_t* )0x1FFFF7CC );
#endif
#if	defined(STM32F407xx)
	uint16_t	size = *( ( uint16_t* )0x1FFF7A22 );
#endif

	printf( "Flash Size   : %u Kbytes", size );
}

static void Print_System_Clock()
{
	printf( "System Clock : %lu MHz", SystemCoreClock / 1000 / 1000 );
}

static void Print_Device_ID()
{
	printf( "Device ID    : %08lx", HAL_GetDEVID() );
}

static void Print_Option_Bytes()
{
	printf( "User Data    : " );

#ifdef	OB_DATA_ADDRESS_DATA0
	printf
	(
		"%lX %lX",
		HAL_FLASHEx_OBGetUserData( OB_DATA_ADDRESS_DATA0 ),
		HAL_FLASHEx_OBGetUserData( OB_DATA_ADDRESS_DATA1 )
	);
#else
	printf( "No user data" );
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
