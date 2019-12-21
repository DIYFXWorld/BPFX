#ifndef	Menu_Item_Interface_h
#define	Menu_Item_Interface_h

#include "FX_ID.h"
#include "FX_Interface.h"

struct Menu_Item_Interface
{
	FX_Interface	*pFX;

	Menu_Item_Interface() : pFX( 0 )
	{
	}

	virtual ~Menu_Item_Interface()
	{
	}

	virtual FX_Interface* Create_FX() = 0;

	virtual void Destroy_FX()
	{
		if( pFX )
		{
			pFX->Destroy();
			pFX = 0;
		}
	}

	virtual void Print_Paramaters() = 0;
	virtual void Set_Param( char c, int v ) = 0;
	virtual void Print_Caption() = 0;
};

#endif
