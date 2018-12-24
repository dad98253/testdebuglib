/*
 * jtrunwind.cpp
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


#ifndef WINDOZE		// this routine does not apply to windows or android
#ifndef __clang__

#include "debug.h"
#include "debug01.h"
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <string.h>

extern int locateLineNum ( long long unsigned int lluRelAddress , char * szFunctionName , char * szExeFile );

char * szMainExeFile = NULL;

char * jtrunwind (int icall) {
  unw_cursor_t cursor; unw_context_t uc;
  unw_word_t ip, sp;
#define JTRUNWINDSIZE	100
  static char ctemp[JTRUNWINDSIZE];
  int j = 0;
  ctemp[0] = '\000';

  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);
  while (unw_step(&cursor) > 0) {
    unw_word_t  offset;
    char        fname[64];
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    fname[0] = '\0';
    unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);
    if (icall < 0 ) {
	dfprintf (__LINE__,__FILE__,TRACEUNWIND,"ip = %lx, sp = %lx (%s+0x%lx)\n", (long) ip, (long) sp, fname, offset);
    }
    if (icall ==  j) {
	if ( strlen( fname ) < (JTRUNWINDSIZE-23) ) {
//		strcpy ( ctemp , fname );
		snprintf ( ctemp ,JTRUNWINDSIZE-1, "%s, +0x%0lx",fname, offset);
	}
    }
    j++;
  }
  return (ctemp);
}

#ifndef NODWARFLIB

char * jtrunwindln (int icall) {
  unw_cursor_t cursor; unw_context_t uc;
  unw_word_t ip, sp;
#define JTRUNWINDSIZE	100
  static char ctemp[JTRUNWINDSIZE];
  int j = 0;
  ctemp[0] = '\000';
  int linenumber = 0;

  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);
  while (unw_step(&cursor) > 0) {
    unw_word_t  offset;
    char        fname[64];
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    fname[0] = '\0';
    unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);
    if (icall < 0 ) {
    	dfprintf (__LINE__,__FILE__,TRACEUNWINDLN,"ip = %lx, sp = %lx (%s+0x%lx)\n", (long) ip, (long) sp, fname, offset);
    	dfprintf (__LINE__,__FILE__,TRACEUNWINDLN,"szMainExeFile = %s\n", szMainExeFile);
    	dfprintf (__LINE__,__FILE__,TRACEUNWINDLN,"line number = %i\n", locateLineNum ( offset ,  (char *)fname ,  szMainExeFile ));
    }
    if (icall ==  j) {
		if ( strlen( fname ) < (JTRUNWINDSIZE-23) ) {
//			strcpy ( ctemp , fname );
			;
			if ( (linenumber = locateLineNum ( offset ,  (char *)fname ,  szMainExeFile)) ) {
				snprintf ( ctemp ,JTRUNWINDSIZE-1, "%s, line %i",fname,linenumber);
			} else {
				snprintf ( ctemp ,JTRUNWINDSIZE-1, "%s, line ?",fname);
			}
		}
    }
    j++;
  }
  return (ctemp);
}

#endif	// NODWARFLIB

#endif  // clang

#endif	// WINDOZE
