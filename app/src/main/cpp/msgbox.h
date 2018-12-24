/*
 * msgbox.h
 *
 *  Created on: May 25, 2018
 *      Author: dad
 */

#ifndef MSGBOX_H_
#define MSGBOX_H_


#ifndef MSGBOXMAIN
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


EXTERN int MsgBox(const char * message, ...);
EXTERN int ErrorMessageTX(LPSTR msg, unsigned int mode);
EXTERN int MessageBoxTX(const char * msg, const char * lpszBanner, unsigned int mode);
EXTERN int SysLogMessage (  LPSTR msg, unsigned int mode );
#ifdef WINDOZE
EXTERN char *strchrnul(const char *s, int c);
#ifndef TEXTMODEONLY
EXTERN int GetConsoleWindowSize(int * rows, int * columns );
#endif // TEXTMODEONLY
#endif // WINDOZE

EXTERN int ErrorMessage(HWND hWnd, LPSTR msg, unsigned int mode);


#endif /* MSGBOX_H_ */
