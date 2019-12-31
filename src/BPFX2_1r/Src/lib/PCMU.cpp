#include	"PCMU.h"

static constexpr uint16_t PCMU_LEVEL[ 8 ] =
{
	0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF
};

int16_t PCMU_Decode( uint8_t code )
{
	int16_t		sample;
	uint8_t		sign, exponent, mantissa;
	int16_t 	magnitude;

	code			=  ~code;
	sign			=   code & 0x80;
	exponent	= ( code >> 4 ) & 0x07;
	mantissa	=   code & 0x0F;

	magnitude	= ( ( ( ( int )mantissa << 3 ) + 0x84 ) << exponent ) - 0x84;

	if( sign == 0x80 )	sample = -( int16_t )magnitude;
  else								sample =  ( int16_t )magnitude;

	return sample;
}

uint8_t PCMU_Encode( int16_t sample )
{
	uint8_t		code;
	uint8_t		sign, exponent, mantissa;
	int16_t		magnitude;

	if( sample < 0 )	{		magnitude = -sample;		sign = 0x80;	}
  else							{		magnitude =  sample;		sign = 0x00;	}

  magnitude += 0x84;

	if( magnitude > 0x7FFF )	magnitude = 0x7FFF;

	for( exponent = 0; exponent < 8; ++exponent )
		if( magnitude <= PCMU_LEVEL[ exponent ] )
			break;

  mantissa = ( magnitude >> ( exponent + 3 ) ) & 0x0F;
	code = ~( sign | ( exponent << 4 ) | mantissa );

	return code;
}
