#ifndef	FX_Menu_h
#define	FX_Menu_h

#include	"Array.h"
#include	"Menu_Item_Interface.h"

struct FX_Menu
{
	Array<Menu_Item_Interface*>		Items;
	int														Count;
	int														Index;

	FX_Menu( int length );

	void Push_Back( Menu_Item_Interface* v );

	FX_Interface* Select( int idx );

	void Set_Param( char c, int v );

	void Print_FX_List();

	void Print_FX_Caption();
};

#endif
