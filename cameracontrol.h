#ifndef _CAMERACONTROL_H_
#define _CAMERACONTROL_H_

#include	<stdlib.h>
#include	<stdio.h>
#include	<windows.h>
#include	<tchar.h>
#include	<time.h>
#include	<math.h>
#include	"maid3.h"
#include	"maid3d1.h"
#include	"CtrlSample.h"

extern "C" __declspec(dllexport) BOOL __cdecl initializeCamera();
extern "C" __declspec(dllexport) BOOL __cdecl finalizeCamera();
extern "C" __declspec(dllexport) BOOL __cdecl getCapture(int resolutionType,char *nikonName);
 BOOL ModulsAndOpenNikon();
 BOOL ModulsAndCloseNikon();
 BOOL openLiveView();
 BOOL closeLiveView();


#endif