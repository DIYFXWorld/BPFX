#ifndef Menu_Item_FX_None_H_
#define Menu_Item_FX_None_H_

#include	"Menu_Item_Interface.h"
#include	"vcp_mini.h"
#include	"FX_None.h"

struct Menu_Item_FX_None : public Menu_Item_Interface
{
	Menu_Item_FX_None()
	{
	}

	virtual FX_Interface* Create_FX()
	{
		pFX = new FX_None;
		return pFX;
	}

	virtual void Set_Param( char, int )
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

#endif
