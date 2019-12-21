#include	"ADPCM_IMA.h"
#include 	"Myutil.h"

static const int ADPCM_Index_Table[ 16 ] = 
{
	-1, -1, -1, -1, 2, 4, 6, 8,
  -1, -1, -1, -1, 2, 4, 6, 8
};

static const int ADPCM_Step_Size_Table[ 89 ] =
{
      7,     8,     9,    10,    11,    12,    13,    14,
     16,    17,    19,    21,    23,    25,    28,    31,
     34,    37,    41,    45,    50,    55,    60,    66,
     73,    80,    88,    97,   107,   118,   130,   143,
    157,   173,   190,   209,   230,   253,   279,   307,
    337,   371,   408,   449,   494,   544,   598,   658,
    724,   796,   876,   963,  1060,  1166,  1282,  1411,
   1552,  1707,  1878,  2066,  2272,  2499,  2749,  3024,
   3327,  3660,  4026,  4428,  4871,  5358,  5894,  6484,
   7132,  7845,  8630,  9493, 10442, 11487, 12635, 13899,
  15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
  32767
};

	ADPCM_IMA_Decoder::ADPCM_IMA_Decoder():
		sp( 0 ), index( 0 )
	{
	}

	int ADPCM_IMA_Decoder::Process( unsigned char c )
	{
	 	int step_size = ADPCM_Step_Size_Table[ index ];

  	int dp = step_size >> 3;

	 	if( ( c & 0x1 ) == 0x1 ) dp += ( step_size >> 2 );
 		if( ( c & 0x2 ) == 0x2 ) dp += ( step_size >> 1 );
	  if( ( c & 0x4 ) == 0x4 ) dp += step_size;
  	if( ( c & 0x8 ) == 0x8 ) sp -= dp;
	  else                     sp += dp;

		sp = Limit( -32768, sp, 32767 );

	  index += ADPCM_Index_Table[ c ];

		index = Limit( 0, index, 88 );

	  return sp;
	}

	void ADPCM_IMA_Decoder::Reset()
	{
		sp = 0;
		index = 0;
	}

	ADPCM_IMA_Encoder::ADPCM_IMA_Encoder():
		sp( 0 ), index( 0 )
	{
	}

	unsigned char ADPCM_IMA_Encoder::Process( int input )
	{
  	unsigned char c;
		int d = input - sp;

		if (d < 0) { c = 0x8; d = -d; }
	  else       { c = 0x0;         }

  	int step_size = ADPCM_Step_Size_Table[ index ];

	  if (d >=   step_size        ) { c |= 0x4; d -=   step_size;        }
		if (d >= ( step_size >> 1 ) ) { c |= 0x2; d -= ( step_size >> 1 ); }
	 	if (d >= ( step_size >> 2 ) ) { c |= 0x1;                          }

  	int dp = step_size >> 3;

		if( ( c & 0x1 ) == 0x1 ) dp += ( step_size >> 2 ) ;
 		if( ( c & 0x2 ) == 0x2 ) dp += ( step_size >> 1 );
	  if( ( c & 0x4 ) == 0x4 ) dp +=   step_size;
  	if( ( c & 0x8 ) == 0x8 ) sp -= dp;
	  else                     sp += dp;

		sp = Limit( -32768, sp, 32767 );

	  index += ADPCM_Index_Table[ c ];

		index = Limit( 0, index, 88 );

		return c;
	}

	void ADPCM_IMA_Encoder::Reset()
	{
		sp = 0;
		index = 0;
	}

