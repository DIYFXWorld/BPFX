#ifndef Q15T_Wave_Table_h
#define	Q15T_Wave_Table_h

#include	"Q15T.h"

#define	COS( v )						Q15T_Get_Wave_Table_Data( v, int16_t_Cos_Table )
#define	SIN( v )						Q15T_Get_Wave_Table_Data( v, int16_t_Sin_Table )
#define	MODULATED_SIN( v )	Q15T_Get_Wave_Table_Data( v, int16_t_Modulated_Sin_Table )

Q15T Q15T_Get_Wave_Table_Data( uint16_t idx, const int16_t* const Table );

template <typename T>
inline int Interpolate_256_Table( int i, const T* const Table )
{
	int	decimal = i & 0xff;
	int	idx = i >> 8;
	return Table[ idx ] + ( Table[ idx + 1 ] - Table[ idx ] ) * decimal / 256;
}

extern const int16_t	int16_t_Cos_Table[ 257 ];
extern const int16_t	int16_t_Sin_Table[ 257 ];
extern const int16_t	int16_t_Modulated_Sin_Table[ 257 ];

extern const	Q15T	Q15T_90R;
extern const	Q15T	Q15T_180R;
extern const	Q15T	Q15T_120R;
extern const	Q15T	Q15T_240R;
extern const	Q15T	Q15T_270R;

#endif
