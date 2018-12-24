/*
 * debug_dump_stuff_f.cpp
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

void debug_dump_stuff_f(unsigned int line, const char * file, unsigned int debugflag, void* x, unsigned int size)
{
	unsigned int dbflg = NOHEADspecial;

	if ( debugflag <  HEADUNCOND    ) dbflg += debugflag;
	if ( debugflag == HEADUNCOND    ) dbflg  = NOHEADspecial;
	if ( debugflag >= NOHEADspecial ) dbflg  = debugflag;
 
	if ( !debugCheckflags( debugflag ) ) return;

	debug_dump_stuff_noeol_f(line,file,dbflg,x,size);
	dfprintf(line,file,dbflg,"\n");
}
