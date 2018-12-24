/*
 * DumpRamFile.cpp
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
#include <string.h>

void DumpRamFile(char **lpHeapFile)
{
	int istart;
	int iend;

	if (*lpHeapFile == NULL) return;

	istart = 1;
	iend = ReAllocateCharVector((unsigned char **)lpHeapFile, 0);

	while ( istart < iend )
	{
		if (strcmp( ((*lpHeapFile)+istart) , "$e$n$d$o$f$f$i$l$e$" ) == 0 ) goto done;
		ddfprintf( ((*lpHeapFile)+istart) );
		istart+= ( strlen( (*lpHeapFile)+istart ) + 1 );
	}

done:
	free((void*)*lpHeapFile);
	*lpHeapFile = NULL;

	return;
}

