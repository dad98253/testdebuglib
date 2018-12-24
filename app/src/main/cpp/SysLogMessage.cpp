
#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "My Secret Box version 2.0" )
#pragma subtitle( "Copyright (c) 2003 - 2015, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#pragma comment(lib, "advapi32.lib")
#endif

//#include "StdAfx.h"
#ifdef WINDOZE
#include <stdio.h>
//#include "passlinEventProvider.h"
#else
#include <syslog.h>
#endif
#include <string.h>
//#include "passwin.h"
//#include "options.h"
//#include "globals.h"
#include "msgbox01.h"

int SysLogMessage (  LPSTR msg, unsigned int mode ) {
	unsigned int LogMessageType;
	LogMessageType = mode%SYSLOGMESSAGE;

#ifdef WINDOZE
#define PROVIDER_NAME "SecretBoxEventProvider"
    HANDLE hEventLog = NULL;
    LPSTR pInsertStrings[2] = {NULL, NULL};
    DWORD dwEventDataSize = 0;
	WORD wEventType = EVENTLOG_INFORMATION_TYPE;
	WORD wCategory = 0;      // event category
	DWORD dwEventID = 0;     // event identifier
// set the event category
	wCategory = (LogMessageType>>4)&&0xffL;
// set the event ID
	dwEventID = (LogMessageType>>12)&&0xfffL;
// set event type - mask off any extra windows categoy or identifier info
	LogMessageType = LogMessageType%0x10L;
// determine the importance of the message
	if ( LogMessageType >  4 ) wEventType = EVENTLOG_ERROR_TYPE; // Error event 
	if ( LogMessageType == 4 ) wEventType = EVENTLOG_WARNING_TYPE; // Warning event 
	if ( LogMessageType <  4 ) wEventType = EVENTLOG_INFORMATION_TYPE; // Information event 
	if ( LogMessageType == 9 ) wEventType = EVENTLOG_AUDIT_SUCCESS; // Success Audit event 
	if ( LogMessageType >  9 ) wEventType = EVENTLOG_AUDIT_FAILURE; // Failure Audit event 


    // The source name (provider) must exist as a subkey of Application.
    hEventLog = RegisterEventSource(NULL, PROVIDER_NAME);
    if (NULL == hEventLog)
    {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"RegisterEventSource failed in SysLogMessage with 0x%x.\n", GetLastError());
#endif
        goto cleanup;
    }

    // This event uses insert strings.
    pInsertStrings[0] = msg;
    if (!ReportEvent(hEventLog, wEventType, wCategory, dwEventID, NULL, 1, 0, (LPCSTR*)pInsertStrings, NULL))
    {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"ReportEvent failed in SysLogMessage with 0x%x for wEventType 0x%x, wCategory 0x%x, dwEventID 0x%x.\n", GetLastError(), wEventType, wCategory, dwEventID);
		dfprintf(__LINE__,__FILE__,TRACE,"msg = \"%s\".\n", msg);
#endif
        goto cleanup;
    }

#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"event successfully reported.\n");
#endif

cleanup:

    if (hEventLog)
        DeregisterEventSource(hEventLog);

#else
// unix :
	int iLogLevel = LOG_INFO;
// set unix log level - mask off any extra windows categoy or identifier info
	LogMessageType = LogMessageType%0x10L;
//This determines the importance of the message. The levels are, in order of decreasing importance:
	if ( LogMessageType == 8 ) iLogLevel = LOG_EMERG; // system is unusable
	if ( LogMessageType == 7 ) iLogLevel = LOG_ALERT; // action must be taken immediately
	if ( LogMessageType == 6 ) iLogLevel = LOG_CRIT; // critical conditions
	if ( LogMessageType == 5 ) iLogLevel = LOG_ERR; // error conditions
	if ( LogMessageType == 4 ) iLogLevel = LOG_WARNING; // warning conditions
	if ( LogMessageType == 3 ) iLogLevel = LOG_NOTICE; // normal, but significant, condition
	if ( LogMessageType == 2 ) iLogLevel = LOG_INFO; // informational message
	if ( LogMessageType == 1 ) iLogLevel = LOG_DEBUG; // debug-level message 
// write message to log file
		 openlog("secretbox", LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT, LOG_USER);
	 	 syslog(iLogLevel, "%s", msg);
	 	 closelog();
#endif // WINDOZE

	return (0);
}
