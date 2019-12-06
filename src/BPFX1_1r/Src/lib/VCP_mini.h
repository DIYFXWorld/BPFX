#ifndef VCP_mini_H_
#define VCP_mini_H_

#include "hal.h"
#include "hal_uart.h"

#define	DEC	10
#define	HEX	16
#define	BIN	2

void Welcome( const char* );

void print( char );
void print( unsigned char );
void print( const char* );
void print( const unsigned char* );
void print( int, int = DEC );
void print( uint16_t, int = DEC );
void print( uint32_t, int = DEC );

void println( char );
void println( unsigned char );
void println( const char* );
void println( const unsigned char* );
void println( int, int = DEC );
void println( uint16_t, int = DEC );
void println( uint32_t, int = DEC );

#endif /* VCP_H_ */
