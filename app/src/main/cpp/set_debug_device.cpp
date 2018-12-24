/*
 * set_debug_device.cpp
 *
 *  Created on: May 24, 2018
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
#include <string.h>


int set_debug_device (char * devicetype )
{
	int truesize;
	int i;
	if ( devicetype == NULL ) return (-1);
	if ( ( truesize = strlen(devicetype) ) == 0 ) return (-2);
	if ( ( truesize = strlen(devicetype) ) > 10 ) return (-3);
	for ( i=0; i<DIMDEBUGDEVICES ; i++ ) {
		if ( strcmp ( devicetype, debug_devices[i] ) == 0 ) {
			iDebugOutputDevice = i;
			if ( OpenDebugDevice() != 1 ) {
				printf ("failed to open debug device\n");
				return (-4);
			}
			return(0);
		}
	}
	return (-5);
}



