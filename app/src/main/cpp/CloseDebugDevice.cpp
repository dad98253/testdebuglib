/*
 * CloseDebugDevice.cpp
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


int CloseDebugDevice()
{
    unsigned long numout;

	switch (iDebugOutputDevice)
      {
      case FILEOUTPUT:
		fclose(hpfile);
        break;

      case SERIALPORT:
		if ( ColorDebug ) {
			if ( ClearDebugScreenOnExit%2 && !bMyWay ) {
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

		FlushFileBuffers(hCom);
		break;

	  case CONSOLE:
		if ( ColorDebug ) {
			hStderr = GetStdHandle(STD_ERROR_HANDLE);
			if (hStderr == INVALID_HANDLE_VALUE)
			{
				MsgBox("GetStdHandle for Console failed, GetLastError=%i,\n disabling future writes",GetLastError());
				bMyWay=FALSE;
				return (-1);
			}
			if ( ClearDebugScreenOnExit%2 && !bMyWay ) {
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
		FreeConsole();
		break;

	  case TCPPORT:

		if ( ClearDebugScreenOnExit%2 && !bMyWay ) if ( ColorDebug ) IPSend((char *)(DBGHOMECURSOR DBGCLRSCR), DBGHOMECURSORLEN + DBGCLRSCRLEN + 2);
		if ( ColorDebug ) IPSend((char *)(DBGRESETALLATTRIBUTES), DBGRESETALLATTRIBUTESLEN + 2);
		ClosePort();

		break;

	  case BITBUCKET:
  	  case DEBUGWIN:
	  case RAM:
		break;

	  default:
        break;
	}
	return 1;
}
