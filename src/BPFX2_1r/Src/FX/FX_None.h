#ifndef	FX_None_h
#define	FX_None_h

#include "FX_Interface.h"
#include "Array.h"
#include "FX_Config.h"

struct FX_None : public FX_Interface
{
//	Array<int16_t>	buf;

	FX_None()
//		buf( FX_NONE_BUFFER_LENGTH )
	{
//		buf.Reset();
	}

	int Process( int )
	{
		asm( "NOP" );
		return 0;
	}

	void Reset() {}

	FX_ID Get_FX_ID() const	{ return FX_ID_None; }
};

#endif
