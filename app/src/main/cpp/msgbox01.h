/*
 * msgbox01.h
 *
 *  Created on: May 25, 2018
 *      Author: dad
 */

#ifndef MSGBOX01_H_
#define MSGBOX01_H_


#ifdef WINDOZE
#include <windows.h>
#else	// WINDOZE
#include "lindows.h"
#endif	//WINDOZE
#include "tutlib.h"


#undef EXTERN
#undef INITIZERO
#undef INITSZERO
#undef INITBOOLFALSE
#undef INITBOOLTRUE
#undef INITNULL
#undef INITNEGDONE

#ifndef MSGBOX01MAIN
#define EXTERN		extern
#define INITIZERO
#define INITSZERO
#define INITBOOLFALSE
#define INITBOOLTRUE
#define INITNULL
#define INITNEGDONE
#else
#define EXTERN
#define INITIZERO	=0
#define INITSZERO	={0}
#define INITBOOLFALSE	=false
#define INITBOOLTRUE	=true
#define INITNULL	=NULL
#define INITNEGDONE	=-1
#endif

#define MIN(x, y) (((x) > (y)) ? (y) : (x))

extern bool bTextMode;

extern char * lpszMsgBoxBanner;
extern HWND hWnd;


#ifdef WINDOZE
extern	HWND hConWindowHandle;		// defined in RedirectIOToConsole in tutlib
extern	HANDLE hTutConsoleBuffer;	// defined in RedirectIOToConsole in tutlib
extern	float fConWindowRowScale;	// defined in RedirectIOToConsole in tutlib
extern	float fConWindowColumnScale;	// defined in RedirectIOToConsole in tutlib
#endif	// WINDOZE


#define SYSLOGMESSAGE	0x01000000L
// SYSLOGMESSAGE is set greater than the largest (documented) windows messagebox option

#undef EXTERN
#undef INITIZERO
#undef INITSZERO
#undef INITBOOLFALSE
#undef INITBOOLTRUE
#undef INITNULL
#undef INITNEGDONE

#endif /* MSGBOX01_H_ */
