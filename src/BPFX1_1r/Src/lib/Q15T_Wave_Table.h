#ifndef Q15T_Wave_Table_h
#define	Q15T_Wave_Table_h

#include	"Q15T.h"

#define	COS( v )						Q15T_Get_Wave_Table_Data( v, int16_t_Cos_Table )
#define	SIN( v )						Q15T_Get_Wave_Table_Data( v, int16_t_Sin_Table )
#define	MODULATED_SIN( v )	Q15T_Get_Wave_Table_Data( v, int16_t_Modulated_Sin_Table )

Q15T Q15T_Get_Wave_Table_Data( uint16_t idx, const int16_t* const Table );

template <typename T>
inline T Interpolate_256_Table( int i, const T* const Table )
{
	int	decimal = i & 0xff;
	int	idx			= i >> 8;
	return Table[ idx ] + ( Table[ idx + 1 ] - Table[ idx ] ) * decimal / 256;
}

template <typename T>
inline T Interpolate_128_Table( int i, const T* const Table )
{
	int	decimal = i & 0x1ff;
	int	idx 		= i >> 9;
	return Table[ idx ] + ( Table[ idx + 1 ] - Table[ idx ] ) * decimal / 512;
}

template <typename T>
inline T Interpolate_64_Table( int i, const T* const Table )
{
	int	decimal = i & 0x3ff;
	int	idx 		= i >> 10;
	return Table[ idx ] + ( Table[ idx + 1 ] - Table[ idx ] ) * decimal / 1024;
}

template <typename T>
inline T Interpolate_32_Table( int i, const T* const Table )
{
	int	decimal = i & 0x7ff;
	int	idx 		= i >> 11;
	return Table[ idx ] + ( Table[ idx + 1 ] - Table[ idx ] ) * decimal / 2048;
}

template <typename T>
inline T Interpolate_16_Table( int i, const T* const Table )
{
	int	decimal = i & 0xfff;
	int	idx 		= i >> 12;
	return Table[ idx ] + ( Table[ idx + 1 ] - Table[ idx ] ) * decimal / 4096;
}

template <typename T>
inline T Interpolate_8_Table( int i, const T* const Table )
{
	int	decimal	= i & 0x1fff;
	int	idx 		= i >> 13;
	return Table[ idx ] + ( Table[ idx + 1 ] - Table[ idx ] ) * decimal / 8192;
}

template <typename T>
inline T Interpolate_4_Table( int i, const T* const Table )
{
	int	decimal	= i & 0x3fff;
	int	idx 		= i >> 14;
	return Table[ idx ] + ( Table[ idx + 1 ] - Table[ idx ] ) * decimal / 16384;
}

extern const int16_t	Cos_Table[ 256+1 ];
extern const int16_t	Sin_Table[ 256+1 ];
extern const int16_t	Modulated_Sin_Table[ 256+1 ];

extern const	Q15T	Q15T_R90;
extern const	Q15T	Q15T_R180;
extern const	Q15T	Q15T_R120;
extern const	Q15T	Q15T_R240;
extern const	Q15T	Q15T_R270;

//////////

#define	Interpolate_256_Table_Index( i )	( i >>  8 )
#define	Interpolate_128_Table_Index( i )	( i >>  9 )
#define	Interpolate_64_Table_Index( i )		( i >> 10 )
#define	Interpolate_32_Table_Index( i )		( i >> 11 )
#define	Interpolate_16_Table_Index( i )		( i >> 12 )
#define	Interpolate_8_Table_Index( i )		( i >> 13 )
#define	Interpolate_4_Table_Index( i )		( i >> 14 )

template <typename T> inline T Interpolate_256_Table( int i, const T& v0, const T& v1 ) { return v0 + ( v1 - v0 ) * ( i & 0x00ff ) /  256; }
template <typename T> inline T Interpolate_128_Table( int i, const T& v0, const T& v1 ) { return v0 + ( v1 - v0 ) * ( i & 0x01ff ) /  512; }
template <typename T> inline T Interpolate_64_Table ( int i, const T& v0, const T& v1 ) { return v0 + ( v1 - v0 ) * ( i & 0x03ff ) / 1024; }
template <typename T> inline T Interpolate_32_Table ( int i, const T& v0, const T& v1 ) { return v0 + ( v1 - v0 ) * ( i & 0x07ff ) / 2048; }
template <typename T> inline T Interpolate_16_Table ( int i, const T& v0, const T& v1 ) { return v0 + ( v1 - v0 ) * ( i & 0x0fff ) / 4096; }
template <typename T> inline T Interpolate_8_Table  ( int i, const T& v0, const T& v1 ) { return v0 + ( v1 - v0 ) * ( i & 0x1fff ) / 8192; }
template <typename T> inline T Interpolate_4_Table  ( int i, const T& v0, const T& v1 ) { return v0 + ( v1 - v0 ) * ( i & 0x3fff ) / 16384; }

#endif
