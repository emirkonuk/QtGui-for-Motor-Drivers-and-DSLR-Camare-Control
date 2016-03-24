#ifndef _MOTORCONTROL_H_
#define _MOTORCONTROL_H_

#include	<stdlib.h>
#include	<stdio.h>
#include	<windows.h>

extern "C" __declspec(dllexport) BOOL __cdecl initializeMotor();
 BOOL OpenPort();
extern "C" __declspec(dllexport) BOOL __cdecl moveTo(int Direction, int Steps);
extern "C" __declspec(dllexport) BOOL __cdecl readPort();

#endif