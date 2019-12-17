#ifndef	Q15T_Cos_Sin_h
#define	Q15T_Cos_Sin_h

#include	"Q15T.h"

Q15T Q15T_Cos( uint16_t i );					// argument is a 0...65535
Q15T Q15T_Sin( uint16_t i );					// argumemt is a 0...65535

inline uint16_t	Q15T_Radian_to_uint16_t( const Q15T& r )
{
	return ( uint64_t )r.Value * 65535 / 205887;				// 205887 = 2 * PI
}

inline Q15T Q15T_Cos_R( const Q15T& r )				// argument is a radian
{
	return Q15T_Cos( Q15T_Radian_to_uint16_t( r ) );
}

inline Q15T Q15T_Sin_R( const Q15T& r )				// argument is a radian
{
	return Q15T_Sin( Q15T_Radian_to_uint16_t( r ) );
}
/*
inline Q15T Q15_Tan( uint16_t i )
{
	return Q15T_Sin( i ) / Q15T_Cos( i );
}
*/
#endif
