#ifndef	FX_None_h
#define	FX_None_h

#include	"FX_Interface.h"

struct FX_None : public FX_Interface
{
	FX_None(){}

	void Destroy() { delete this; }

	int Process( int ) { return 0;}

	void Set_Control_0( int )	{}
	int Get_Control_0() const	{ return 0; }

	void Set_Control_1( int ) {}
	int Get_Control_1() const	{ return 0; }

	void Set_Control_2( int )	{}
	int Get_Control_2() const	{ return 0;}

	void Set_Control_3( int )	{}
	int Get_Control_3() const	{ return 0;}

	void Set_Control_4( int )	{}
	int Get_Control_4() const	{ return 0;}

	int Get_Memory_Length() const { return 0; }

	enum_FX_ID Get_ID() const	{ return FX_ID_None; }
};

#endif
