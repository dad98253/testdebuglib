/*
 * jtrbacktrace.cpp
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

#ifndef WINDOZE		// this routine does not work on windows
#ifndef __clang__		// this routine does not work on android

#include "debug.h"
#include "debug01.h"
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>



char * jtrbacktrace(int i)
{
    int j; 
    int nptrs;
#define BTSIZE 100
    void *buffer[BTSIZE];
    char **strings;
    static char ctemp[BTSIZE];

   nptrs = backtrace(buffer, BTSIZE);
//    printf("backtrace() returned %d addresses\n", nptrs);

   /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

   strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

   if ( i == 0 ) for (j = 0; j < nptrs; j++)
        fprintf(stderr,"%s\n", strings[j]);

   ctemp[0] = '\000';
   if ( i > -1 && i <= nptrs ) if ( strlen( strings[i] ) < (BTSIZE-1) ) strcpy ( ctemp , strings[i] ); 
   free(strings);


   return (ctemp);
}

#endif  // clang
#endif	// WINDOZE
