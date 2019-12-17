#ifndef FX_Interface_H_
#define FX_Interface_H_

#include	"FX_ID.h"

struct FX_Interface
{
	bool		Reset;

	FX_Interface() : Reset( false ){}

	virtual ~FX_Interface(){}

	virtual 	int Process( int ) = 0;

	virtual void Set_Control_0( int ) = 0;
	virtual int  Get_Control_0() const = 0;

	virtual void Set_Control_1( int ) = 0;
	virtual int  Get_Control_1() const = 0;

	virtual void Set_Control_2( int ) = 0;
	virtual int  Get_Control_2() const = 0;

	virtual void Set_Control_3( int ) = 0;
	virtual int  Get_Control_3() const = 0;

	virtual void Set_Control_4( int ) = 0;
	virtual int  Get_Control_4() const = 0;

	virtual int Get_Memory_Length() const = 0;

	virtual enum_FX_ID Get_ID() const = 0;

	virtual void Destroy() = 0;
};

#endif /* FX_BASE_H_ */
