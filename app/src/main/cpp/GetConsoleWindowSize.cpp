#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "My Secret Box version 2.0" )
#pragma subtitle( "Copyright (c) 2003 - 2015, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif

#ifdef WINDOZE
#include <windows.h>
#include <stdio.h>
#include "msgbox01.h"

int GetConsoleWindowSize(int * rows, int * columns ) 
{

	CONSOLE_SCREEN_BUFFER_INFO csbi;



	if ( hConWindowHandle == NULL ) {
		MessageBox(NULL,"Console window not open\nThis is a programming error...\nSecret Box will now terminate","Program Error",MB_OK);
		return(-1);
	}


    if ( !GetConsoleScreenBufferInfo(hTutConsoleBuffer, &csbi) ) {
		DWORD derror;
		char buffer[100];
		derror = GetLastError();
		if (sprintf (buffer, "Call to GetConsoleScreenBufferInfo failed\nError number = %d", derror) > 99 ) {
			MessageBox(NULL,"Bad value returned from GetLastError in GetConsoleWindowSize\nThis is a programming error...\nSecret Box will now terminate","Program Error",MB_OK);
			return (-2);
		}
		MessageBox(NULL,buffer,"Program Error",MB_OK);
		return ((int)derror); 
	}
//    *columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
//    *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;


    RECT r;
    GetClientRect(hConWindowHandle, &r); //gets the console's current dimensions 

	*columns = (int)(fConWindowColumnScale * ((float)r.right ));
	*rows    = (int)(fConWindowRowScale    * ((float)r.bottom));

#ifdef DEBUGCONSOLEWINDOW
    printf("columns: %d\n", *columns);
    printf("rows: %d\n", *rows);
#endif

    return (0);

}
#endif	// WINDOZE
