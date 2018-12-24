/*
 * debug_dump_stuff_msg_f.cpp
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


void debug_dump_stuff_msg_f(unsigned int line, const char * file, unsigned int debugflag, const void *msg, void *x, unsigned int size)
{
	unsigned int dbflg  = NOHEADspecial;
	unsigned int dbflg2 = NOHEADspecial;

	if ( debugflag <  HEADUNCOND    ) dbflg += debugflag;
	if ( debugflag == HEADUNCOND    ) dbflg  = NOHEADspecial;
	if ( debugflag >= NOHEADspecial ) dbflg  = debugflag;
	if ( debugflag <= HEADUNCOND    ) dbflg2 = debugflag;
	if ( debugflag >  NOHEADspecial ) dbflg2 -= debugflag;

	if ( !debugCheckflags( debugflag ) ) return;

	dfprintf(__LINE__,__FILE__,DEBUGDMPSTUFF,"debug_dump_stuff_msg_f: line = %i, file = %s, debugflag = %u, msg = %s, dbflg = %u, dbflg2 = %u\n",
									line, file, debugflag, msg, dbflg , dbflg2 );

	if ( x ) {
		dfprintf(line,file,dbflg2,"%s[0-%i] starting at address %p: \n",(char *)msg ,size ,x );
		debug_dump_stuff_f(line,file,dbflg,x , size);
	} else {
		dfprintf(line,file,dbflg2,"%s points to -nil-, dump of %s[0-%i] not possible!\n", (char *)msg, (char *)msg, size);
	}

	return;
}
