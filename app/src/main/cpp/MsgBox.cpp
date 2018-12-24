#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "My Secret Box version 2.0" )
#pragma subtitle( "Copyright (c) 2003 - 2015, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "debug.h"
#ifdef WINDOZE
#include <windows.h>
#else
#include "lindows.h"
#endif

#define MSGBOX01MAIN
#include "msgbox01.h"

//#include "StdAfx.h"
//#include "resource.h"
//#include "passwin.h"
//#include "globals.h"
//#include "options.h"

#define MIN(x, y) (((x) > (y)) ? (y) : (x))

bool bTextMode = false;

char * lpszMsgBoxBanner = NULL;
HWND hWnd = 0;

extern int ErrorMessageTX(LPSTR msg, unsigned int mode);
extern int ErrorMessage(HWND hWnd, LPSTR msg, unsigned int mode);

/*
How MsgBox works:
	The simplest way to use MsgBox is to just call it with a single string argument. In this case,
	MsgBox will produce a message box with an "error" banner and a single "ok" button.
	The string sent to MsgBox can also contain formatting instructions. Note that not all C fprintf
	formatting is recognized (see the code below), but most of the more common ones (strings,integers,
	floats, hex, etc.) are. The call to MsgBox has a variable argument list. The arguments beyond the format
	string are assumed to be the proper kind of data associated with the formats found in the format string.
	
	  If the last two characters of the format string are special formats type "%&", then an additional
	unsigned integer argument is expected. This argument must appear before any other additional (formatting)
	arguments in the list and it will replace the default messagebox mode (MB_ICONERROR | MB_OK)
	used by MsgBox. If the "%&" mode is specified, then the global char* variable lpszMsgBoxBanner
	(found in globals.h) will be checked. If is not NULL, then the string that it points to will be used as
	the title of message box. If it is NULL, then the title of message box will be blank.
	
	  If the last two characters of the format string are special formats type "%?" or "%!", then the message is
	assumed to be a yes/no question or an informational message respectfully. For yes/no questions, the title
	bar will be set to "Select Yes or No" and the mode will be set to MB_YESNO | MB_ICONQUESTION. For informational
	messages, he title bar will be set to "Information" and the mode will be set to MB_OK | MB_ICONEXCLAMATION.
 

	MsgBox will always return the value of the return from MessageBox.

  examples:

	MsgBox("program error");
	MsgBox("program error %i", error_num);
	if ( MsgBox("Do you want some info?%?") == IDYES ) MsgBox("Here is some info: %s%!", "some info");
	lpszMsgBoxBanner = "Very Bad Error!";
	while ( MsgReturn == IDNO ) {
		MsgReturn = 
		MsgBox ("Do you wish to create the %s instructions file?%&", MB_YESNOCANCEL | MB_ICONQUESTION , "recover");
	}
	lpszMsgBoxBanner = NULL;
	if ( MsgReturn == IDCANCEL ) NumBackups--;


  Note: The default MessageBox created is modal. The global variable HWND hWnd (found in passwin.h) is always passed
		to MessageBox as the owner window. This can cause a problem if hWnd is not set to a valid window
		handle. In some cases, one may wish to create a message box before any other window has been created. To
		avoide this problem, passwin creates a message only window early on (see routine passwin.cpp). However,
		this also complicates the programming, in that we must change hWnd to point to whoever is going to be 
		the owner of error message boxes as we flow through the program... This is a lot of work, just to avoid
		needing to pass the owner window handle into MsgBox each time it is called. The goal was to keep the MsgBox
		call as simple as possible. It works great in a "normal" Windows program where a master window is opened
		up as soon as the WinMain program starts followed by a message loop. The passwin program is different: WinMain
		has no message loop; the "main" window is a dialog box.

*/





int MsgBox(const char * fmt, ... )
{
	unsigned int mode = 0;
//	bool ismode = false;
	int lastchar;
	va_list ap;
	va_start(ap, fmt);
	lastchar = strlen(fmt);
	if (lastchar > 0 ) {
		if ( fmt[lastchar-1] == '&' ) {
			mode = va_arg(ap, unsigned int);
		} 
	}
	if (lastchar == 0 ) return -1;

    char *p, *sval;
    int ival;
	unsigned int uival;
    double dval;
#define BUFFERLEN	2000
	char buffer[BUFFERLEN];
//	buffer[0]='\000';
	char *b;
	char *saveme = NULL;
	char cTrue[] = "true";
	char cFalse[] = "false";
	b=buffer;
	if ( bTextMode ) {
		*b = ' ';
		b++;
	}
	*b = '\000';
	int len;
	int templen;
//MessageBox( hWnd, "testing MessageBox at start of MsgBox", NULL, MB_ICONERROR | MB_OK );
    for (p = (char*)fmt; *p; p++) {         /* [1] */
        if (*p != '%') {
            *b = *p;
			b++;
            continue;                /* [2] */
        }
        switch (*++p) {
        case 'i':
            ival = va_arg(ap, int);
            len = sprintf(b, "%i", ival);
			b+=len;
            break;
        case 'f':
            dval = va_arg(ap, double);
            len = sprintf(b, "%f", dval);
			b+=len;
            break;
        case 's':
			sval = va_arg(ap, char *);
			templen = strlen(sval);
			if (templen < BUFFERLEN*8/10 || templen > 0) strncpy(b, sval, MIN(templen,BUFFERLEN*8/10));   // why is this "||" ??? should it be "&&"?
			b+=templen;
            break;
        case 'b':
			ival = va_arg(ap, int);
			if (ival) {
				templen = 4;
				if (templen < BUFFERLEN*8/10 || templen > 0) strncpy(b, cTrue, MIN(templen,BUFFERLEN*8/10));
				b+=templen;
			} else {
				templen = 5;
				if (templen < BUFFERLEN*8/10 || templen > 0) strncpy(b, cFalse, MIN(templen,BUFFERLEN*8/10));
				b+=templen;
			}
            break;
        case 'c':
            ival = va_arg(ap, int);
            len = sprintf(b, "%c", ival);
			b+=len;
            break;
        case 'd':
            ival = va_arg(ap, int);
            len = sprintf(b, "%d", ival);
			b+=len;
            break;
        case 'o':
            ival = va_arg(ap, int);
            len = sprintf(b, "%o", ival);
			b+=len;
            break;
        case 'u':
            uival = va_arg(ap, unsigned int);
            len = sprintf(b, "%u", uival);
			b+=len;
            break;
        case 'x':
            ival = va_arg(ap, int);
            len = sprintf(b, "%x", ival);
			b+=len;
            break;
        case 'X':
            ival = va_arg(ap, int);
            len = sprintf(b, "%X", ival);
			b+=len;
            break;
        case 'e':
            dval = va_arg(ap, double);
            len = sprintf(b, "%e", dval);
			b+=len;
            break;
        case 'E':
            dval = va_arg(ap, double);
            len = sprintf(b, "%E", dval);
			b+=len;
            break;
        case 'g':
            dval = va_arg(ap, double);
            len = sprintf(b, "%g", dval);
			b+=len;
            break;
        case 'G':
            dval = va_arg(ap, double);
            len = sprintf(b, "%G", dval);
			b+=len;
            break;
        case '&':
//			++p;
            break;
        case '?':
			mode = MB_YESNO | MB_ICONQUESTION;
			saveme = lpszMsgBoxBanner;
			lpszMsgBoxBanner = (char*)"Select Yes or No";
//			++p;
            break;
        case '!':
			mode = MB_OK | MB_ICONEXCLAMATION;
			saveme = lpszMsgBoxBanner;
			lpszMsgBoxBanner = (char*)"Information";
//			++p;
            break;
		case '\0':
			p--; // reverse the ++p from the header of the switch
			break;
        default:
            *b = *p;
			b++;
            break;
        }
    }

	*b = '\0';

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE, "%s\n", buffer);
#endif

//	fflush(stdout);
	va_end(ap);
	int iSaveReturn = ErrorMessage( hWnd,  buffer, mode);
	lpszMsgBoxBanner = saveme;
	return (iSaveReturn);
}

