/*
 * debug_dump_stuff_msg.cpp
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

/*
void debug_dump_stuff_msg(const void *msg, void *x, unsigned int size)
{

	unsigned int i;
//printf(" ->x = %p\n",x);
	if ( x ) {
		dfprintf(__LINE__,__FILE__,TRACE,"dump of %s[0-%i] starting at address %p: \n", (char *)msg, size, x);
		for(i=0;i<size;i++)
		{
			dfprintf(__LINE__,__FILE__,NOHEADspecial,"%02x", ((unsigned char*)x)[i]);
			if( (i%4)==3 ) {
				dfprintf(__LINE__,__FILE__,NOHEADspecial," ");
			}
		}
		dfprintf(__LINE__,__FILE__,NOHEADspecial," ___ ");
		for(i=0;i<size;i++)
		{
			if ( isprint(((unsigned char*)x)[i]) ) {
				dfprintf(__LINE__,__FILE__,NOHEADspecial,"%c", ((unsigned char*)x)[i]);
			} else {
				dfprintf(__LINE__,__FILE__,NOHEADspecial,".");
			}
			if( (i%4)==3 ) {
				dfprintf(__LINE__,__FILE__,NOHEADspecial," ");
			}
		}
		dfprintf(__LINE__,__FILE__,NOHEADspecial,"\n");
	} else {
		dfprintf(__LINE__,__FILE__,TRACE,"%s points to NULL, dump of [0-%i] not possible!\n", (char *)msg, size);
	}

	return;
}
*/

