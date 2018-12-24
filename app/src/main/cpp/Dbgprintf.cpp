/*
 * Dbgprintf.cpp
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
#include <assert.h>
#include <string.h>


int Dbgprintf(int linenum, const char * modulename, unsigned int debugflag, const char * fmt,...)
{
va_list args;

int nTmp,jkTmp;
unsigned long numout;
HANDLE hStderr;

    va_start(args, fmt);

	if ( !bMyWay ) return -1;
	if ( debugflag > NUMDEBUGFLAGS && ( debugflag < NOHEADspecial || debugflag > ( NOHEADspecial + NUMDEBUGFLAGS ) ) ) {
		MsgBox("Dbgprintf at %i in %s failed, \"%u\" is not a valid debug flag number\n debug flag numbers must be less than %u",linenum,modulename,debugflag,NUMDEBUGFLAGS);
		bMyWay=FALSE;
		return(-1);
	}
	if ( debugflag == 0 ) {
		return(0);
	}
	if ( debugflag < HEADUNCOND ) if ( bdebug_flag_set[debugflag] == 0 ) {
		return(0);
	}
	if ( debugflag > NOHEADspecial ) if ( bdebug_flag_set[debugflag-NOHEADspecial] == 0 ) {
		return(0);
	}

	if ( !debugCheckflags( debugflag ) ) return(0);
	str[0]='\000';
	tmpstr[0]='\000';
	nTmp=vsprintf(tmpstr,fmt,args);
	assert(nTmp<LENTEMPSTR);
	if ( debugflag >= NOHEADspecial ) {
		nTmp=sprintf(str,"%s",tmpstr);
	} else {
		char * strmodulename = NULL;
		char * dotloc = NULL;
		char shortname[16];
#ifdef WINDOZE
		strmodulename = strrchr ( (char *)modulename , '\\' )+1;
#else	// WINDOZE
		strmodulename = strrchr ( (char *)modulename , '/' )+1;
#endif	// WINDOZE
		if ( strmodulename == NULL ) strmodulename = (char *)modulename; 
		dotloc = strrchr ( strmodulename , '.' );
		if ( dotloc == NULL ) dotloc = strmodulename + MIN(strlen(strmodulename),15);
		strncpy(shortname,(const char *)strmodulename,(size_t)(dotloc-strmodulename));
		shortname[(dotloc-strmodulename)]='\000';
		nTmp=sprintf(str,DBGBOLDGREEN(@) DBGBOLDRED(%i) DBGBOLDGREEN(>) DBGBOLDRED(%s) ": %s",linenum,shortname,tmpstr);
	}
	assert(nTmp<LENTEMPSTR);
	switch (iDebugOutputDevice)
      {
      case FILEOUTPUT:
		if ( !ColorDebug ) {
			strcpy(tmpstr,str);
			StripCSI(tmpstr,str);
			nTmp=strlen(str);
		}
//		str[nTmp]='\n';
//		nTmp++;
		str[nTmp]='\000';
//		  if ((int)fwrite(str, sizeof(str[0]), nTmp, hpfile) != nTmp ) {
		  if ((jkTmp=(int)fprintf(hpfile,"%s",str)) != nTmp ) {
			 fflush(hpfile);
			 MsgBox("Write to debug file failed, GetLastError=%i,\n disabling future writes",GetLastError());
			 bMyWay=FALSE;
			return (-1);
		  }
		fflush(hpfile);
        break;

      case SERIALPORT:
		if ( !ColorDebug ) {
			strcpy(tmpstr,str);
			StripCSI(tmpstr,str);
			nTmp=strlen(str);
		}
		str[nTmp]='\r';
		nTmp++;
		if ( !WriteFile( hCom, str, nTmp, &numout, NULL) )
		{
			 MsgBox("WriteFile to COM port failed, GetLastError=%i,\n disabling future writes",GetLastError());
			 bMyWay=FALSE;
			return (-1);
		}
		break;

	  case CONSOLE:
//		str[nTmp]='\n';
//		nTmp++;
		if ( !ColorDebug ) {
			strcpy(tmpstr,str);
			StripCSI(tmpstr,str);
		}
		hStderr = GetStdHandle(STD_ERROR_HANDLE);
		if (hStderr == INVALID_HANDLE_VALUE)
		{
			 MsgBox("GetStdHandle for Console failed, GetLastError=%i,\n disabling future writes",GetLastError());
			 bMyWay=FALSE;
			return (-1);
		}
		if ( !WriteFile( hStderr, str, nTmp, &numout, NULL) )
		{
			MsgBox("WriteFile to Console failed, GetLastError=%i,\n disabling future writes",GetLastError());
			bMyWay=FALSE;
			return (-1);
		}
		break;

	  case TCPPORT:

		if ( !ColorDebug ) {
			strcpy(tmpstr,str);
			StripCSI(tmpstr,str);
		}
		IPSend(str, strlen(str)+1);	

		break;

	  case BITBUCKET:
		break;
	  
	  case DEBUGWIN:
		strcpy(tmpstr,str);
		StripCSI(tmpstr,str);
		OutputDebugString(str);
		break;

	  case RAM:
		SaveStringInHeap(&lpHeapFile,str);
		dfprintf(__LINE__,__FILE__,DEBUGDBGPRINTF,"%s",str);

		break;
	}

    va_end(args);
    return(nTmp);
}
