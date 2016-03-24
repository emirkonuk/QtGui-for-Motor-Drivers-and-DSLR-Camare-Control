#include	<stdlib.h>
#include	<stdio.h>
#include	<windows.h>
#include	<tchar.h>
#include	<time.h>
#include	<math.h>
#include	"Maid3.h"
#include	"maid3d1.h"
#include	"CtrlSample.h"

//#include	<iostream>
//#include <core\core_c.h>
char *imname;
LPMAIDEntryPointProc	g_pMAIDEntryPoint = NULL;
BCHAR	g_bFileRemoved = false;
ULONG	g_ulCameraType = 0;	// CameraType

HINSTANCE	g_hInstModule = NULL;
char	buf[256];
UWORD	wSel;
char	ModulePath[MAX_PATH];
LPRefObj	pRefMod , pRefSrc , RefItm , pRefDat, pRefItm ;
ULONG	ulModID = 0, ulSrcID = 0, ulItemID = 0, ulDataType = 0;
BOOL	bRet;

__declspec(dllexport)
BOOL initializeCamera(){
	ModulsAndOpenNikon();
	openLiveView();
}
__declspec(dllexport)
 BOOL finalizeCamera(){
	closeLiveView();
	ModulsAndCloseNikon();
}

BOOL ModulsAndOpenNikon()
{
	// Search for a Module-file like "Type0004.md3".
	bRet = Search_Module( ModulePath );
	if ( bRet == false ) {
		return -1;
	}

	// Load the Module-file.
	bRet = Load_Module( ModulePath );
	if ( bRet == false ) {
		return -1;
	}

	// Allocate memory for reference to Module object.
	pRefMod = (LPRefObj)malloc(sizeof(RefObj));
	if ( pRefMod == NULL ) {
		return -1;
	}
	InitRefObj( pRefMod );

	// Allocate memory for Module object.
	pRefMod->pObject = (LPNkMAIDObject)malloc(sizeof(NkMAIDObject));
	if ( pRefMod->pObject == NULL ) {
		if ( pRefMod != NULL )	free( pRefMod );
		return -1;
	}

	//	Open Module object
	pRefMod->pObject->refClient = (NKREF)pRefMod;
	bRet = Command_Open(	NULL,					// When Module_Object will be opend, "pParentObj" is "NULL".
								pRefMod->pObject,	// Pointer to Module_Object 
								ulModID );			// Module object ID set by Client
	if ( bRet == false ) {
		if ( pRefMod->pObject != NULL )	free( pRefMod->pObject );
		if ( pRefMod != NULL )	free( pRefMod );
		return -1;
	}

	//	Enumerate Capabilities that the Module has.
	bRet = EnumCapabilities( pRefMod->pObject, &(pRefMod->ulCapCount), &(pRefMod->pCapArray), NULL, NULL );
	if ( bRet == false ) {
		if ( pRefMod->pObject != NULL )	free( pRefMod->pObject );
		if ( pRefMod != NULL )	free( pRefMod );
		return -1;
	}

	//	Set the callback functions(ProgressProc, EventProc and UIRequestProc).
	bRet = SetProc( pRefMod );
	if ( bRet == false ) {
		if ( pRefMod->pObject != NULL )	free( pRefMod->pObject );
		if ( pRefMod != NULL )	free( pRefMod );
		return -1;
	}

	//	Set the kNkMAIDCapability_ModuleMode.
	if( CheckCapabilityOperation( pRefMod, kNkMAIDCapability_ModuleMode, kNkMAIDCapOperation_Set )  ){
		bRet = Command_CapSet( pRefMod->pObject, kNkMAIDCapability_ModuleMode, kNkMAIDDataType_Unsigned, 
										(NKPARAM)kNkMAIDModuleMode_Controller, NULL, NULL);
		if ( bRet == false ) {
			return -1;
		}
	}

	return 1;
}
//------------------------------------------------------------------------------------------------------------------------------------
//

BOOL ModulsAndCloseNikon(){
	// Close Module_Object
	bRet = Close_Module( pRefMod );
	// Unload Module
	FreeLibrary( g_hInstModule );
	g_hInstModule = NULL;

	// Free memory blocks allocated in this function.
	if ( pRefMod->pObject != NULL )	free( pRefMod->pObject );
	if ( pRefMod != NULL )	free( pRefMod );

	return 0;
}
//------------------------------------------------------------------------------------------------------------------------------------
//

BOOL openLiveView(){ 
	bRet = SelectSource( pRefMod, &ulSrcID );
	if( ulSrcID > 0 )
		bRet = SourceCommandLoop( pRefMod, ulSrcID );
	return bRet;
}
//------------------------------------------------------------------------------------------------------------------------------------
//

BOOL closeLiveView(){
	bRet = SetUnsignedCapability( pRefSrc, kNkMAIDCapability_LiveViewStatus ,0);
	// Close Source_Object
	bRet = RemoveChild( pRefMod, ulSrcID );
	return bRet;
}
//------------------------------------------------------------------------------------------------------------------------------------
//
__declspec(dllexport)
BOOL getCapture(int resolutionType, char *nikonName){
	if(resolutionType == 1){
		bRet = GetLiveViewImageCapability( pRefSrc ,nikonName);
		nikonName ="";
	}
	if(resolutionType == 0){
		imname = nikonName;
		bRet = IssueProcess( pRefSrc, kNkMAIDCapability_Capture );
		Command_Async( pRefSrc->pObject );
		WaitEvent();
		// Children
		// Select Item  Object
		ulItemID = 0;
		bRet = SelectItem( pRefSrc, &ulItemID );
		if( bRet == true && ulItemID > 0 )
		bRet = ItemCommandLoop( pRefSrc, ulItemID );
		WaitEvent();
	}

	return bRet;
}
//------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------
//
BOOL SourceCommandLoop( LPRefObj pRefMod, ULONG ulSrcID)
{
	LPRefObj	pRefSrc = NULL;
	char	buf[256];
	ULONG	ulItemID = 0;
	UWORD	wSel;
	BOOL	bRet = true;
	

	pRefSrc = GetRefChildPtr_ID( pRefMod, ulSrcID );
	if ( pRefSrc == NULL ) {
		// Create Source object and RefSrc structure.
		if ( AddChild( pRefMod, ulSrcID ) == true ) {
		} else {
			return false;
		}
		pRefSrc = GetRefChildPtr_ID( pRefMod, ulSrcID );
	}

	// Get CameraType
	Command_CapGet( pRefSrc->pObject, kNkMAIDCapability_CameraType, kNkMAIDDataType_UnsignedPtr, (NKPARAM)&g_ulCameraType, NULL, NULL );

	// command loop
				
	SetShootingMenu( pRefSrc);
	WaitEvent();
				
	return true;
}
//------------------------------------------------------------------------------------------------------------------------------------
//
BOOL SetShootingMenu( LPRefObj pRefSrc1) 
{
	// LiveViewStatus
	pRefSrc = pRefSrc1;
	bRet = SetUnsignedCapability( pRefSrc1, kNkMAIDCapability_LiveViewStatus ,1);
	//GetLiveViewImageCapability( pRefSrc );
	//SetUnsignedCapability( pRefSrc, kNkMAIDCapability_LiveViewStatus ,0);
	return bRet;
}
//------------------------------------------------------------------------------------------------------------------------------------
//
BOOL ItemCommandLoop( LPRefObj pRefSrc, ULONG ulItmID )
{

	pRefItm = GetRefChildPtr_ID( pRefSrc, ulItmID );
	/*
	if ( pRefItm == NULL ) {
		// Create Item object and RefSrc structure.
		if ( AddChild( pRefSrc, ulItmID ) == true ) {
			printf("Item object is opened.\n");
		} else {
			printf("Item object can't be opened.\n");
			return false;
		}
		pRefItm = GetRefChildPtr_ID( pRefSrc, ulItmID );
	}
	*/
	// command loop
	// Show Children
	// Select Data Object
	ulDataType = 0;
	bRet = SelectData( pRefItm, &ulDataType );
	if ( bRet == false )	return false;
	if ( ulDataType == kNkMAIDDataObjType_Image )
	{
		// reset file removed flag
		g_bFileRemoved = false;
		bRet = ImageCommandLoop( pRefItm, ulDataType );
		// If the image data was stored in DRAM, the item has been removed after reading image.
		if ( g_bFileRemoved ) {
			RemoveChild( pRefSrc, ulItmID );
			pRefItm = NULL;
		}
	}
	if ( pRefItm != NULL ) {
		// If the item object remains, close it and remove from parent link.
		//////////////////////////////////////
		bRet = RemoveChild( pRefSrc, ulItmID );
	}
	return bRet;
}
//------------------------------------------------------------------------------------------------------------------------------------
//
BOOL ImageCommandLoop( LPRefObj pRefItm, ULONG ulDatID )
{

	pRefDat = GetRefChildPtr_ID( pRefItm, ulDatID );
	/*
	if ( pRefDat == NULL ) {
		// Create Image object and RefSrc structure.
		if ( AddChild( pRefItm, ulDatID ) == true ) {
			printf("Image object is opened.\n");
		} else {
			printf("Image object can't be opened.\n");
			return false;
		}
		pRefDat = GetRefChildPtr_ID( pRefItm, ulDatID );
	}
	*/
	// command loop
	// Acquire
	bRet = IssueAcquire( pRefDat );
	// The item has been possibly removed.
	// Close Image_Object

	///////////
	bRet = RemoveChild( pRefItm, ulDatID );
	return bRet;
}
//------------------------------------------------------------------------------------------------------------------------------------
//
