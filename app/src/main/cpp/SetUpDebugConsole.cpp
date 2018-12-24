/*
 * SetUpDebugConsole.cpp
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
 
int SetUpDebugConsole(void) 
{ 
    LPSTR lpszPrompt1 = (char*)"Debug console opened for passwin";
    LPSTR lpszPrompt2 = (char*)"(keyboard input to this window is ignored)\n";
//    CHAR chBuffer[256]; 
//    DWORD cRead; 
    DWORD cWritten, fdwMode, fdwOldMode; 
    WORD wOldColorAttrs;
    COORD dwScreenBufferSize; 

	FreeConsole();

	if (!AllocConsole())
    {
        MsgBox("AllocConsole failed"); 
    }

	hConsoleBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,  // access flag
		0,      // buffer share mode   can be zero | FILE_SHARE_READ | FILE_SHARE_WRITE
		NULL , // pointer to security attributes  LPSECURITY_ATTRIBUTES *lpSecurityAttributes
		CONSOLE_TEXTMODE_BUFFER,          // type of buffer to create   The only currently supported screen buffer type is CONSOLE_TEXTMODE_BUFFER.
		NULL   // reserved   LPVOID lpScreenBufferData
	);

    // Get handles to STDIN and STDOUT. 

    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	hStderr = GetStdHandle(STD_ERROR_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE || 
        hStdout == INVALID_HANDLE_VALUE || hStderr == INVALID_HANDLE_VALUE) 
    {
        MsgBox("GetStdHandle failed");
        return 1;
    }
    // Save the current text colors. 

    if (! GetConsoleScreenBufferInfo(hConsoleBuffer, &csbiInfo)) 
    {
        MsgBox("GetConsoleScreenBufferInfo failed in SetUpDebugConsole"); 
        return 1;
    }

    wOldColorAttrs = csbiInfo.wAttributes; 

    // Set the text attributes to draw red text on black background. 

    if (! SetConsoleTextAttribute(hStderr, FOREGROUND_RED | 
            FOREGROUND_INTENSITY))
    {
        MsgBox("SetConsoleTextAttribute failed in SetUpDebugConsole"); 
        return 1;
    }

    // Set the screen buffer size to 150 columns by 250 lines. 

	dwScreenBufferSize.X = 150;
	dwScreenBufferSize.Y = 250;

    if (! SetConsoleScreenBufferSize(hStderr, dwScreenBufferSize) )
    {
        MsgBox("SetConsoleScreenBufferSize failed in SetUpDebugConsole"); 
        return 1;
    }

    // Write to STDERR by using the default 
    // modes. Input is echoed automatically, and ReadFile 
    // does not return until a carriage return is typed. 
    // 
    // The default input modes are line, processed, and echo. 
    // The default output modes are processed and wrap at EOL. 

        if (! WriteFile( 
            hStderr,               // output handle 
            lpszPrompt1,           // prompt string 
            lstrlenA(lpszPrompt1), // string length 
            &cWritten,             // bytes written 
            NULL) )                // not overlapped 
        {
            MsgBox("WriteFile failed in SetUpDebugConsole"); 
            return 1;
        }


    // Turn off the line input and echo input modes 

    if (! GetConsoleMode(hConsoleBuffer, &fdwOldMode)) 
    {
       MsgBox("GetConsoleMode failed in SetUpDebugConsole");
       return 1;
    }

    fdwMode = fdwOldMode & 
        ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT); 
    if (! SetConsoleMode(hConsoleBuffer, fdwMode)) 
    {
       MsgBox("SetConsoleMode failed in SetUpDebugConsole");
       return 1;
    }

    // ReadFile returns when any input is available.  
    // WriteFile is used to echo input. 

    NewLine();

        if (! WriteFile( 
            hStderr,               // output handle 
            lpszPrompt2,           // prompt string 
            lstrlenA(lpszPrompt2), // string length 
            &cWritten,             // bytes written 
            NULL) )                // not overlapped 
        {
            MsgBox("second WriteFile failed in SetUpDebugConsole");
            return 1;
        }
/*
        if (! ReadFile(hStdin, chBuffer, 1, &cRead, NULL)) 
            break; 
        if (chBuffer[0] == '\r')
            NewLine();
        else if (! WriteFile(hStdout, chBuffer, cRead, 
            &cWritten, NULL)) break;
        else
            NewLine();
        if (chBuffer[0] == 'q') break; 
*/
    // Restore the original console mode. 

    SetConsoleMode(hConsoleBuffer, fdwOldMode);

    // Restore the original text colors. 

    SetConsoleTextAttribute(hStderr, wOldColorAttrs);

    return 0;
}
