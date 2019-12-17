#ifndef Menu_FX_None_H_
#define Menu_FX_None_H_

#include <FX_None.h>
#include	"Menu_Interface.h"
#include	"..\FX_Process.h"

struct Menu_FX_None : public Menu_Interface
{
	Menu_FX_None()
	{
	}

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_None; }

	virtual void Active()
	{
		Select_FX( FX_ID_None );
	}

	virtual void Set_Control( char c, int v )
	{
	}

	virtual void Print_Paramaters()
	{
		printf( "No Params\n" );
	}

	virtual void Print_Caption()
	{
		printf( "FX None" );
	}
};

#endif /* Menu_FX_None_H_ */
