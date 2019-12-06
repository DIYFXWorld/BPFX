#ifndef Menu_FX_None_H_
#define Menu_FX_None_H_

#include	"..\FX_Process.h"
#include	"FX_None.h"
#include	"Menu_Interface.h"
#include	"vcp_mini.h"

struct Menu_FX_None : public Menu_Interface
{
	Menu_FX_None()
	{
	}

	virtual void Active()
	{
		Select_FX( FX_ID_None );
	}

	virtual void Set_Param( char c, int v )
	{
	}

	virtual void Print_Paramaters()
	{
		println( "No Params" );
	}

	virtual void Print_Caption()
	{
		print( "FX None" );
	}
};

#endif /* Menu_FX_None_H_ */
