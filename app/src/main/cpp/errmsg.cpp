#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "My Secret Box version 2.0" )
#pragma subtitle( "Copyright (c) 2003 - 2015, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif

#include <string.h>
//#include "StdAfx.h"
//#include "resource.h"
//#include "passwin.h"
//#include "globals.h"
//#include "options.h"

#include "msgbox01.h"

#ifdef WINDOZE
#define JKNULL	NULL
#endif	// WINDOZE

int ErrorMessageTX(LPSTR msg, unsigned int mode);
extern int ErrorMessage(HWND hWnd, LPSTR msg, unsigned int mode);

int MessageBoxTX(const char * msg, const char * lpszBanner, unsigned int mode);
int SysLogMessage (  LPSTR msg, unsigned int mode );
#ifdef WINDOZE
char *strchrnul(const char *s, int c);
#ifndef TEXTMODEONLY
int GetConsoleWindowSize(int * rows, int * columns );
#endif // TEXTMODEONLY
#endif // WINDOZE

int ErrorMessage(HWND hWnd, LPSTR msg, unsigned int mode)
{
	int msgboxID = 0;
//MessageBox( hWnd, "testing MessageBox at start of ErrorMessage", NULL, MB_ICONERROR | MB_OK );  
	if ( mode == 0 ) {
		if ( bTextMode ) {
			ErrorMessageTX(msg,mode);
			return(IDOK);
#ifndef TEXTMODEONLY
		} else {
			msgboxID = MessageBox( hWnd, (const char *)msg, NULL, MB_ICONERROR | MB_OK );
#endif
		}
	} else {
		if ( bTextMode ) {
			msgboxID = ErrorMessageTX(msg, mode);
#ifndef TEXTMODEONLY
		} else {
			if ( mode >= SYSLOGMESSAGE ) {
				SysLogMessage ( msg, mode );
			} else {
				if ( lpszMsgBoxBanner == NULL ) {
					msgboxID = MessageBox( hWnd, (const char *)msg, " ", mode );
				} else {
					msgboxID = MessageBox( hWnd, (const char *)msg, lpszMsgBoxBanner, mode );
				}
			}
#endif
		}
	}
    switch (msgboxID)
    {
    case IDCANCEL:
        // TODO: add code
        break;
    case IDOK:
        // TODO: add code
        break;
	case JKNULL:
		return 0;
    }

    return msgboxID;
}

int ErrorMessageTX(LPSTR msg, unsigned int mode) {
	int msgboxID = 0;
	if ( mode >= SYSLOGMESSAGE ) {
		SysLogMessage ( msg, mode );
	} else {
		if ( lpszMsgBoxBanner == NULL ) {
			msgboxID = MessageBoxTX( (const char *)msg, " ", mode );
		} else {
			msgboxID = MessageBoxTX( (const char *)msg, lpszMsgBoxBanner, mode );
		}
	}
	return(msgboxID);
}

char * FindWordBreak (char * msg, int iLen) {
	int i;
	if (iLen < 1) return(NULL);
	int iRealLen = MIN(iLen,(int)strlen(msg));

	for (i=iRealLen-1;i>0;i--) if ( msg[i] == ' ' ) return (msg+i);

	return (msg+iRealLen-1);
}

int FormatTutMsg (const char * msg) {
#define TUTLINELEN	80
#define TUTLINEBUFSIZE	150
	int iLineLength;
	int iNextCR;
	int iNextWorkBrk;
	char * szStartMsg;
	char szTutString[TUTLINEBUFSIZE];
	char *chrptr;
	char *chrWordBrk;
	char *szNullChar;

	szStartMsg = (char *)msg;
	szNullChar = szStartMsg + strlen (szStartMsg);
// determine how long to make the lines
	iLineLength = TUTLINELEN - 1;
#ifdef WINDOZE
#ifndef TEXTMODEONLY
	int rows,columns;
	if (!GetConsoleWindowSize(&rows, &columns )) {
		iLineLength = MIN(TUTLINEBUFSIZE-1,columns);
	}
#endif // TEXTMODEONLY
#endif // WINDOZE

	

	while ( szNullChar > szStartMsg ) {
		chrptr = (char *)strchrnul(szStartMsg, '\n');
		iNextCR = (chrptr - szStartMsg);
		if ( iNextCR < iLineLength ) {
			strncpy(szTutString,(const char *)szStartMsg, (size_t)iNextCR);
			szTutString[iNextCR] = '\000';
		} else {
			chrWordBrk = FindWordBreak(szStartMsg, iLineLength );
			iNextWorkBrk = (chrWordBrk - szStartMsg);
			strncpy(szTutString,(const char *)szStartMsg, (size_t)MIN(iNextCR,iNextWorkBrk));
			if (MIN(iNextCR,iNextWorkBrk) < iLineLength) szTutString[MIN(iNextCR,iNextWorkBrk)] = '\000';
			if (MIN(iNextCR,iNextWorkBrk) == iLineLength) szTutString[iLineLength-1] = '\000';
		}
		tutPrintString(szTutString);
		if ( strlen(szTutString) == strlen(szStartMsg) ) break;  // avoide over indexing szStartMsg at while() test
		szStartMsg = szStartMsg + strlen(szTutString) + 1;
	}

	return 0;
}

int MessageBoxTX(const char * msg, const char * lpszBanner, unsigned int mode) {
	if ( mode & (MB_YESNO | MB_ICONQUESTION) ) {
		FormatTutMsg(msg);
		int iNumArg = 1;
		tutDisplayPrompt ( MESSAGE_BOX , iNumArg , "BANNER", lpszBanner);
		int iReply = (int)tutFetchDatax2 (1);
		if ( iReply == 1 ) return(IDYES);
		return(IDNO);
	}
	if ( mode & ( MB_OK | MB_ICONEXCLAMATION | MB_ICONERROR ) ) {
		if ( strcmp( lpszBanner , " " ) != 0 ) tutPrintString(lpszBanner);
		FormatTutMsg(msg);
		return(IDOK);
	}
// default:
	if ( strcmp( lpszBanner , " " ) != 0 ) tutPrintString(lpszBanner);
	FormatTutMsg(msg);
	return(IDOK);
}

#ifdef WINDOZE
char *strchrnul(const char *s, int c){
	char * p;

	if ( (p = strchr(s, c)) == NULL ) p = (char *)s + (unsigned int)strlen(s);

	return (p);

}
#endif
