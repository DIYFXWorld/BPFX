#ifndef	Memory_Pool_h
#define	Memory_Pool_h

#include <stdint.h>

void Set_Memory_Pool( void* memory, uint32_t bytes );
void Set_Memory_Pool( uint32_t bytes );

void* __Malloc__( uint32_t bytes );
void __Free__( void* );

#endif

