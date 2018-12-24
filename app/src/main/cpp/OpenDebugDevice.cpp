/*
 * OpenDebugDevice.cpp
 *
 *  Created on: May 16, 2018
 *      Author: dad
 */

#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "debuglib version 2.0" )
#pragma subtitle( "Copyright (c) 2018, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif


#include "debug.h"
#include "debug01.h"
#include <string.h>

int OpenDebugDevice()
{
    DCB dcb;
    bool fSuccess;
    unsigned long numout;
#define LENTEMPLINE	250
	char *line = NULL;

	SaveStringInHeap(&lpHeapFile,(char*)"$e$n$d$o$f$f$i$l$e$");

	switch (iDebugOutputDevice)
      {

      case FILEOUTPUT:
			if ( lpDebugOutputFileName == NULL ) {
				if ((hpfile = fopen("debug.txt", "w")) == NULL) {
					MsgBox("unable to open debug.txt file.");
					return 0;
				}
			} else {
				if ((hpfile = fopen(lpDebugOutputFileName, "w")) == NULL) {
					MsgBox("unable to open debug.txt file.");
					return 0;
				}
			}
			break;

      case SERIALPORT:
			line = (char *)realloc( line , LENTEMPLINE);

			if ( GetProfileString ("john","DefaultComPort","",line,LENTEMPLINE+2) ==
                                                             0 )
			{
				if (lpDebugOutputFileName == NULL ) {
					MsgBox("Open COM port failed, GetLastError=%i\n\0",GetLastError());
					bMyWay=FALSE;
					return 0;
				}
				strncpy(line,lpDebugOutputFileName,LENTEMPLINE);
			}
			hCom = CreateFile(line,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,
                       0,NULL);
			free(line);

			if (hCom==INVALID_HANDLE_VALUE)
			{
			   MsgBox("CreateFile failed, GetLastError=%i\n\0",GetLastError());
			   bMyWay=FALSE;
			  return 0;
			}

//    fSuccess=PurgeComm(hCom,
//                       PURGE_TXABORT|
//                       PURGE_RXABORT|
//                       PURGE_TXCLEAR|
//                       PURGE_RXCLEAR);
//    if (!fSuccess)
//    {
//        MsgBox("PurgeComm failed, GetLastError=%i\n\0",GetLastError());
//        bMyWay=FALSE;
//        return;
//    }

			fSuccess=SetupComm(hCom,1600,1600);
			if (!fSuccess)
			{
				MsgBox("SetupComm failed, GetLastError=%i\n\0",GetLastError());
				bMyWay=FALSE;
				return 0;
			}

			fSuccess=GetCommState(hCom, &dcb);
			if (!fSuccess)
			{
				MsgBox("GetCommState failed, GetLastError=%i\n\0",GetLastError());
				bMyWay=FALSE;
				return 0;
			}

			dcb.BaudRate=4800;
			dcb.ByteSize=8;
			dcb.Parity=NOPARITY;
			dcb.StopBits=ONESTOPBIT;

			fSuccess=SetCommState(hCom,&dcb);
			if (!fSuccess)
			{
				MsgBox("SetCommState failed, GetLastError=%i\n\0",GetLastError());
				bMyWay=FALSE;
				return 0;
			}
			if ( ColorDebug ) {
				if ( ClearDebugScreenOnExit > 1 ) {
					if ( !WriteFile( hCom, DBGHOMECURSOR DBGCLRSCR, DBGHOMECURSORLEN + DBGCLRSCRLEN + 1, &numout, NULL) )
					{
						 MsgBox("WriteFile to COM port failed, GetLastError=%i,\n disabling future writes",GetLastError());
						 bMyWay=FALSE;
						return (-1);
					}
				}
				if ( !WriteFile( hCom, DBGRESETALLATTRIBUTES, DBGRESETALLATTRIBUTESLEN + 1, &numout, NULL) )
				{
					 MsgBox("WriteFile to COM port failed, GetLastError=%i,\n disabling future writes",GetLastError());
					 bMyWay=FALSE;
					return (-1);
				}
			}

			break;

	  case CONSOLE:

			SetUpDebugConsole();
			if ( ColorDebug ) {
				hStderr = GetStdHandle(STD_ERROR_HANDLE);
				if (hStderr == INVALID_HANDLE_VALUE)
				{
					MsgBox("GetStdHandle for Console failed, GetLastError=%i,\n disabling future writes",GetLastError());
					bMyWay=FALSE;
					return (-1);
				}
				if ( ClearDebugScreenOnExit > 1 ) {
					if ( !WriteFile( hStderr, DBGHOMECURSOR DBGCLRSCR, DBGHOMECURSORLEN + DBGCLRSCRLEN + 1, &numout, NULL) )
					{
						MsgBox("WriteFile to Console failed, GetLastError=%i,\n disabling future writes",GetLastError());
						bMyWay=FALSE;
						return (-1);
					}
				}
				if ( !WriteFile( hStderr, DBGRESETALLATTRIBUTES, DBGRESETALLATTRIBUTESLEN + 1, &numout, NULL) )
				{
					MsgBox("WriteFile to Console failed, GetLastError=%i,\n disabling future writes",GetLastError());
					bMyWay=FALSE;
					return (-1);
				}
			}

			break;

	  case TCPPORT:

			if (OpenTCPport()==0) return 0;
			if ( ClearDebugScreenOnExit > 1 ) if ( ColorDebug ) IPSend((char *)(DBGHOMECURSOR DBGCLRSCR), DBGHOMECURSORLEN + DBGCLRSCRLEN + 2);
			if ( ColorDebug ) IPSend((char *)(DBGRESETALLATTRIBUTES), DBGRESETALLATTRIBUTESLEN + 2);

			break;

	  case BITBUCKET:
	  case DEBUGWIN:
	  case RAM:
			break;

	}

	DumpRamFile(&lpHeapFile);

    return 1;
}
