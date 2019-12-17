#ifndef	Menu_Interface_h
#define	Menu_Interface_h

#include	"FX_ID.h"

struct Menu_Interface
{
	virtual ~Menu_Interface(){}

	virtual enum_FX_ID Get_ID() const = 0;

	virtual void Active() = 0;
	virtual void Print_Paramaters() = 0;
	virtual void Set_Control( char c, int v ) = 0;
	virtual void Print_Caption() = 0;
};

#endif
