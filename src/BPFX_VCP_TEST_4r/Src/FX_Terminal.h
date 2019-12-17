/*
 * Terminal_Control.h
 *
 *  Created on: 2018/12/22
 *      Author: gamenyge
 */

#ifndef FX_TERMINAL_H_
#define FX_TERMINAL_H_

extern bool			Clear_All;

void Print_Usage();
void Print_Paramaters();
void Get_Terminal_Value();

void Set_Time( int v );
void Set_Feedback( int v );
void Set_Mix_Level( int v );

#endif /* FX_TERMINAL_H_ */
