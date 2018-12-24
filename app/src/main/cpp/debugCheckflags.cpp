/*
 * debugCheckflags.cpp
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


int debugCheckflags( unsigned int debugflag )
{
	if ( !bMyWay ) return(0);
	if ( debugflag == 0 ) return(0);
	if ( debugflag == HEADUNCOND ) return(1); 
	if ( debugflag == NOHEADspecial ) return(1);
	if ( ( debugflag > NUMDEBUGFLAGS && debugflag < HEADUNCOND ) || debugflag > ( NOHEADspecial + NUMDEBUGFLAGS ) ) {
		MsgBox("debugCheckflags failed, \"%u\" is not a valid debug flag number\n debug flag numbers must be less than %u",debugflag,NUMDEBUGFLAGS);
		if ( ! bdebug_flag_set[BMYWAYOVERRIDE] ) bMyWay=FALSE;
		return(0);
	}
	if ( debugflag < HEADUNCOND ) {
		if ( bdebug_flag_set[debugflag] ) return(1);
	}
	if ( debugflag > NOHEADspecial ) {
		if ( bdebug_flag_set[debugflag-NOHEADspecial] ) return(1);
	}

	return (0);
}
