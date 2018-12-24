/*
 * SaveStringInHeap.cpp
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

bool SaveStringInHeap(char **lpHeapFile,char *str)
{
	int istart;
	int iend;
//	dfprintf(__LINE__,__FILE__,DEBUGSAVEINSTRING,"save \"%s\" *lpHeapFile=(0x%llx)\n",str,*lpHeapFile);
	istart = ReAllocateCharVector((unsigned char **)lpHeapFile, 0);
	if ( istart == 1 ) {
		**lpHeapFile = '\n';
		iStartOfHeapFile++;
	}
	if ( istart < (iStartOfHeapFile + (int)strlen(str) + 2)) {
		iend   = ReAllocateCharVector((unsigned char **)lpHeapFile, strlen(str)+1 );
		if (iend == 0) {
			fprintf(stderr,"realloc failed in SaveStringInHeap\n");
			return (true);
		}
	}
//	dfprintf(__LINE__,__FILE__,DEBUGSAVEINSTRING,"save string at (0x%llx)+%i of length %i\n",*lpHeapFile,istart,(int)strlen(str));
	strcpy(*lpHeapFile+iStartOfHeapFile,str);
	iend = iStartOfHeapFile + strlen(str);
	*(*lpHeapFile+iend) = '\000';
	iStartOfHeapFile+=strlen(str)+1;

	return (false);
}
