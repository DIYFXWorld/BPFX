#ifndef	ADPCM_IMA_H
#define	ADPCM_IMA_H

#include 	"Myutil.h"

struct ADPCM_IMA_Decoder
{
	int sp, index;

	ADPCM_IMA_Decoder();

	int Process( unsigned char c );
	void Reset();
};

struct ADPCM_IMA_Encoder
{
	int		sp, index;

	ADPCM_IMA_Encoder();


	unsigned char Process( int input );

	void Reset();
};

#endif
