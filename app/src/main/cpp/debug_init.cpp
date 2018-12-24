/*
 * debug_init.cpp
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

#define DEBUGMAIN
#include "debug.h"
#include "debug01.h"
#define FROM_DEBUGLIB2
#include "tutlib.h"
#ifndef BSD
#include <malloc.h>
#else	// BSD
#include <stdlib.h>
#endif	// BSD

extern int tutIsTty();

DEBUGOPTIONSTYPE *dbgoptions;

int debug_init ( void **debug_options_struct, int initialvirbositry)
{
	int truesize;
	int retvalue = 0;
	if ( *debug_options_struct == NULL ) {
		printf(" error in debug_init call \n");
		return (-1);
	}
	dbgoptions = (DEBUGOPTIONSTYPE *)debug_options_struct;
	iDebugOutputDevice = 6;	// added why??
	debugInitialVirbosity = debugVirbosity = initialvirbositry;
	bdebug_flag_set[TRACE] = 1;
	debug_read_proc_file(0);
//fprintf(stderr,"debugheapstart = %llx, debugheapend = %llx, debugstackstart = %llx, debugstackend = %llx\n",debugheapstart, debugheapend, debugstackstart, debugstackend);
//exit(EXIT_FAILURE);
	truesize = sizeof(debug_flag)/sizeof(debug_flag[0]);
	if ( truesize != NUMDEBUGFLAGS ) {
		fprintf(stderr,"bad dimention on debug_flag array, true size = %i, NUMDEBUGFLAGS = %i\n",truesize,NUMDEBUGFLAGS);
		retvalue++;
	}
	truesize = sizeof(bdebug_flag_set)/sizeof(bdebug_flag_set[0]);
	if ( truesize != NUMDEBUGFLAGS ) {
		fprintf(stderr,"bad dimention on bdebug_flag_set array, true size = %i, NUMDEBUGFLAGS = %i\n",truesize,NUMDEBUGFLAGS);
		retvalue+=2;
	}
	if (retvalue) return(retvalue);
//char str[LENTEMPSTR];
//char tmpstr[LENTEMPSTR];
	str=(char *)malloc(LENTEMPSTR);
	tmpstr=(char *)malloc(LENTEMPSTR);

	// check if stdin is tty...
	tutIsTty();

	return(0);
}
