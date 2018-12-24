/*
 * debug_read_proc_file.cpp
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

#ifndef WINDOZE		//	this routine does not apply to windows
#include "debug.h"
#include "debug01.h"
#include <stdlib.h>
#include <string.h>



int debug_read_proc_file(int idumpem) 
{
#define MAXPFNAMELEN	100
	char pfname[MAXPFNAMELEN];
	char maploc[MAXPFNAMELEN];
	char mode[MAXPFNAMELEN];
	char *line = NULL;
	FILE *procfile;
	ssize_t linelen;
	size_t maxlen = 0;
	int numread,pfnlen;
	unsigned long long int startadd,endadd;
#ifndef BSD
	unsigned int startoffset,devmaj,devmin,inode;
#else	// BSD
	unsigned int u1,u2,u3,u4,x1;
	unsigned long long int llx;
	int ixflag;
	char COW[10],NC[10],VNODE[10],CH[10];
	unsigned long long int debuguserstart;
	unsigned long long int debuguserend;
	debuguserstart =  0x400000ULL;
	debuguserend =    0x7ffffffffULL;
	debugheapstart =  0x800000000ULL;
	debugheapend =    0x0ULL;
	debugstackstart = 0x800000000000ULL;
	debugstackend =   0x0ULL;
#endif	// BSD
	mypid = getpid();
	myppid = getppid();
#ifndef BSD
	pfnlen = snprintf(pfname,MAXPFNAMELEN,"/proc/%u/maps",(unsigned int)mypid);
#else	// BSD
	pfnlen = snprintf(pfname,MAXPFNAMELEN,"/proc/%u/map",(unsigned int)mypid);
#endif	// BSD
	if (pfnlen > (MAXPFNAMELEN - 2) ) {
		fprintf(stderr,"unable to construct proc file name : %s\n",pfname);
		exit(EXIT_FAILURE);
	}
	if ((procfile = fopen(pfname, "r")) == NULL) {
		fprintf(stderr,"unable to open %s file.\n",pfname);
		exit(EXIT_FAILURE);
	}
	while ( (linelen = getline(&line, &maxlen, procfile)) > 0 ) {
		numread=strlen(line);
		if(numread>0) line[numread-1]='\000';
		if ( idumpem ) dfprintf(__LINE__,__FILE__,NOHEADspecial," %s\n", line);
#ifndef BSD
		numread = sscanf ( line, "%llx-%llx %s %x %x:%x %u %s", &startadd,&endadd,mode,&startoffset,&devmaj,&devmin,&inode,maploc);
		if ( numread != 7 && numread != 8 ) {
#else	// BSD
		numread = sscanf ( line, "%llx %llx %u %u %llx %s %u %u %x %s %s %s %s %s %i",&startadd,&endadd,&u1,&u2,&llx,mode,&u3,&u4,&x1,COW,NC,VNODE,maploc,CH,&ixflag);
		if ( numread != 15 && numread != 14 ) {
#endif	// BSD

			fprintf(stderr,"unable to parse line in /proc/../maps file : %s \n",line);
			fclose(procfile);
			exit(EXIT_FAILURE);
		}

#ifndef BSD
		if (strcmp(maploc,"[heap]") == 0 ) {
			debugheapstart = startadd;
			debugheapend = endadd;			
		}
		if (strcmp(maploc,"[stack]") == 0 ) {
			debugstackstart = startadd;
			debugstackend = endadd;			
		}
#else	// BSD
		if ( startadd > 0x400000000000ULL ) {
			debugstackstart = MIN(startadd,debugstackstart);
			debugstackend = MAX(endadd,debugstackend);
			continue;
		}
		if ( startadd > 0x7ffffffffULL ) {
			debugheapstart = MIN(startadd,debugheapstart);
			debugheapend = MAX(endadd,debugheapend);
			continue;
		}
		if ( startadd > 0x3fffffULL ) {
			debuguserstart = MIN(startadd,debuguserstart);
			debuguserend = MAX(endadd,debuguserend);
			continue;
		}
#endif	// BSD
	}
	free(line);
	fclose(procfile);

	return 0;
}


#else	// WINDOZE

int debug_read_proc_file(int idumpem) 
{
	return 0;
}

#endif	// WINDOZE

