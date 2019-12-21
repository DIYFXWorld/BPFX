#include <stdlib.h>
#include "Memory_Pool.h"

static	void* 		__Memory__;
static	uint32_t	__Length__;

void Set_Memory_Pool( void* memory, uint32_t bytes )
{
	__Memory__ = ( void* )memory;
	__Length__ = bytes;
}

void Set_Memory_Pool( uint32_t bytes )
{
	__Memory__ = malloc( bytes );
	__Length__ = bytes;
}

void* __Malloc__( uint32_t bytes )
{
	if( !__Memory__ )
		while( true ){ /* Error */ }

	if( __Length__ < bytes )
		while( true ){ /* Error */ }

	return __Memory__;
}

void __Free__( void* ){}

