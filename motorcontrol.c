#include	<stdlib.h>
#include	<stdio.h>
#include	<windows.h>
#include	"Maid3.h"
#include	"maid3d1.h"
#include	"CtrlSample.h"

#define READ_TIMEOUT      1 
HANDLE hCom;
__declspec(dllexport)
BOOL initializeMotor(){
	OpenPort();
}
//------------------------------------------------------------------------------------------------------------------------------------
//
//__declspec(dllexport)
BOOL OpenPort(){
	int b=0;
	DCB dcb;
	
	BOOL fSuccess;
	TCHAR *pcCommPort = TEXT("COM3");
	hCom = CreateFile( pcCommPort,
		                 GENERIC_READ | GENERIC_WRITE,
			              0,      //  must be opened with exclusive-access
				          NULL,   //  default security attributes
					      OPEN_EXISTING, //  must use OPEN_EXISTING
						  0,      //  not overlapped I/O
	                      NULL ); //  hTemplate must be NULL for comm devices

   if (hCom == INVALID_HANDLE_VALUE) 
   {
       //  Handle the error.
       printf ("CreateFile failed with error %d.\n", GetLastError());
       return (1);
   }

   //  Initialize the DCB structure.
   SecureZeroMemory(&dcb, sizeof(DCB));
   dcb.DCBlength = sizeof(DCB);

   //  Build on the current configuration by first retrieving all current
   //  settings.
   fSuccess = GetCommState(hCom, &dcb);

   if (!fSuccess) 
   {
      //  Handle the error.
      printf ("GetCommState failed with error %d.\n", GetLastError());
      return (2);
   }

   //  Output to console

   //  Fill in some DCB values and set the com state: 
   //  57,600 bps, 8 data bits, no parity, and 1 stop bit.
   dcb.BaudRate = CBR_9600;     //  baud rate
   dcb.ByteSize = 8;             //  data size, xmit and rcv
   dcb.Parity   = NOPARITY;      //  parity bit
   dcb.StopBits = ONESTOPBIT;    //  stop bit

   fSuccess = SetCommState(hCom, &dcb);

   if (!fSuccess) 
   {
      //  Handle the error.
      printf ("SetCommState failed with error %d.\n", GetLastError());
      return (3);
   }

   //  Get the comm config again.
   fSuccess = GetCommState(hCom, &dcb);

   if (!fSuccess) 
   {
      //  Handle the error.
      printf ("GetCommState failed with error %d.\n", GetLastError());
      return (2);
   }

   //PrintCommState(dcb);       //  Output to console
   _tprintf (TEXT("Serial port %s successfully reconfigured.\n"), pcCommPort);
}
//------------------------------------------------------------------------------------------------------------------------------------
//
__declspec(dllexport)
BOOL readPort(){

DWORD dwRes;
DWORD dwRead;
BOOL fWaitingOnRead = FALSE;
OVERLAPPED osReader = {0};
int readValue;

osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
if (osReader.hEvent == NULL)
    printf("*Error creating overlapped event; abort\n");

if(!ReadFile( hCom, readValue, 1, &dwRead, &osReader)) 
    dwRes = WaitForSingleObject(osReader.hEvent, READ_TIMEOUT);
//printf("%d\n", readValue);
CloseHandle(osReader.hEvent);
}
__declspec(dllexport)
BOOL moveTo(int Direction, int Steps)
{
   OVERLAPPED osWrite = {0};
   DWORD dwWritten;
   DWORD dwRes;
   BOOL fRes;
   DWORD dwToWrite = 1;
   char *lpBuf;
   int i=0;

   // Create this write operation's OVERLAPPED structure's hEvent.
   osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (osWrite.hEvent == NULL)
      // error creating overlapped event handle
      return FALSE;
   
   //Up - Down
   if(Direction == -1){
		lpBuf = "2";
   }
   if(Direction == 1){
		lpBuf = "1";
   }
   //X axis
   if(Direction == 2){
		lpBuf = "3";
   }
   if(Direction == -2){
		lpBuf = "4";
   }
   //Y axis
   if(Direction == 3){
		lpBuf = "5";
   }
   if(Direction == -3){
		lpBuf = "6";
   }
   // Issue write.
   for( i=0; i < Steps; i++)
   {
	   if (!WriteFile( hCom, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
		  if (GetLastError() != ERROR_IO_PENDING) { 
			 // WriteFile failed, but isn't delayed. Report error and abort.
			 fRes = FALSE;
		  }
		  else
			 // Write is pending.
			 dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
			 switch(dwRes)
			 {
				// OVERLAPPED structure's event has been signaled. 
				case WAIT_OBJECT_0:
					 if (!GetOverlappedResult(hCom, &osWrite, &dwWritten, FALSE))
						   fRes = FALSE;
					 else
					  // Write operation completed successfully.
					  fRes = TRUE;
					 break;
            
				default:
					 // An error has occurred in WaitForSingleObject.
					 // This usually indicates a problem with the
					// OVERLAPPED structure's event handle.
					 fRes = FALSE;
					 break;
			 }
		  }
	   else
		  // WriteFile completed immediately.
		  fRes = TRUE;
   }

   CloseHandle(osWrite.hEvent);
   return fRes;
}
