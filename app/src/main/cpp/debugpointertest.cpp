/*
 * debugpointertest.cpp
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

#ifndef WINDOZE		// this routine does not apply to windows

#include "debug.h"
#include "debug01.h"

int debugpointertest ( void *x )
{				// this code only works on linux...
	intptr_t uP1;

	uP1 = (intptr_t)x;
//	dfprintf(__LINE__,__FILE__,TRACE,"debugpointertest called from %s: x -> 0x%016lx\n",jtrunwind(1),uP1);
	if ( uP1 == 0 ) return (0);	// -nil- pointer (bad boy)
	debug_read_proc_file(0);	// get current heap and stack limits
	if (  uP1 < (intptr_t)debugheapend ) return(-1);	// pointer less than end of heap (good boy)
	if ( uP1 > (intptr_t)debugstackend ) return(0);	// pointer above end of syack (bad boy)
	if ( uP1 > (intptr_t)debugstackstart ) return(1);	// pointer less than end of stack and above start of stack (good boy)

	return (0);	// pointer above end of heap and below start of stack (bad boy)
}


#endif	// WINDOZE
