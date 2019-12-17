/*
 * Menu.h
 *
 *  Created on: 2019/06/24
 *      Author: gamenyge
 */

#ifndef MENU_H_
#define MENU_H_

#include	"Menu_Interface.h"

extern Menu_Interface			*Menu;

void Menu_Initial();
void Select_Menu( int index );

void Print_FX_List();
int Get_Current_Index();

#endif /* MENU_H_ */
