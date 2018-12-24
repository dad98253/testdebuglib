#ifndef _DEBUG01_H
#define _DEBUG01_H

#ifndef DEBUGMAIN
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

#ifndef WINDOZE
#define _MSC_VER	0
#endif	// WINDOZE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WINDOZE
// Windows Header Files:
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define UNUSED(x) x
#else	// WINDOZE
// Linux Header Files:
#include "lindows.h"
#endif	// WINDOZE

#ifdef WINDOZE
#include <stdio.h>
//#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#else	// WINDOZE
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#endif	// WINDOZE

#ifndef bool
    #define bool int
    #define false ((bool)0)
    #define true  ((bool)1)
#endif

#define DEFAULT_PORT 5001
#ifdef WINDOZE
#define DEFAULT_PROTO SOCK_DGRAM // UDP
#else
#define DEFAULT_PROTO 2
//#define DEFAULT_PROTO SOCK_DGRAM // UDP
//#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#endif

#ifndef MIN
#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#endif	// MIN
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif	// MAX

EXTERN struct sockaddr_in server;
EXTERN struct hostent *hp;
#ifdef WINDOZE
EXTERN WSADATA wsaData;
EXTERN SOCKET  conn_socket;
#else
EXTERN int  conn_socket;
extern int h_errno;
#endif
EXTERN unsigned int addr;
EXTERN char *server_name;

#ifdef DEBUGMAIN
unsigned short port = DEFAULT_PORT;
int socket_type = DEFAULT_PROTO;
//int socket_type = SOCK_DGRAM;
//int socket_type = DEFAULT_PROTO;
bool fDebug = TRUE;
bool bMyWay = TRUE;
#else	// DEBUGMAIN
EXTERN unsigned short port;
EXTERN int socket_type;
EXTERN bool fDebug;
EXTERN bool bMyWay;
#endif	// DEBUGMAIN
#define LENTEMPSTR	10000
EXTERN char *str;
EXTERN char *tmpstr;
// static variables
//FILE *hf;
EXTERN HANDLE hCom;
EXTERN HANDLE hStdout, hStdin, hStderr;
EXTERN char * lpHeapFile;
EXTERN int iStartOfHeapFile;
EXTERN FILE *hpfile;
EXTERN HANDLE hConsoleBuffer;
EXTERN CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
#ifndef WINDOZE
EXTERN pid_t mypid;
EXTERN pid_t myppid;
#endif	// WINDOZE

#if ARCH_INT_GT_32
typedef unsigned short ARCH_WORD_32;
typedef unsigned int ARCH_WORD_64;
#else	// ARCH_INT_GT_32
typedef unsigned int ARCH_WORD_32;
#ifdef WINDOZE
typedef unsigned __int64 ARCH_WORD_64;
#else	// WINDOZE
typedef unsigned long long ARCH_WORD_64;
#endif	// WINDOZE
#endif	// ARCH_INT_GT_32

typedef ARCH_WORD_64 opt_flags;
/*
 * Structure with debug option flags and all the parameters.
 */
typedef struct {
/* Option flags */
	opt_flags flags;

	/* Password files */
	struct list_main *passwd;

#ifdef DEBUG
/* debug options */
	unsigned int debug_level;
	struct list_main *debug_flags;
	char * debug_server_name;
	unsigned int degug_port_number;
	char * debug_filename;
	char * debug_device;
	unsigned int debug_color_flag;
	unsigned int debug_clear_screen;
	struct list_main *debug_mask_messages;
	struct list_main *debug_display_messages;
#endif

} DEBUGOPTIONSTYPE;

EXTERN char * lpDebugOutputFileName;
// lpDebugOutputFileName is the optional name for the debug output file.
// ==NULL (i.e., a NULL pointer) debug output goes to the default debug output device
// COMx => debug output is to go to a serial port x
// CON  => debug output is to go to a console screen
// PORT => debug output is to go to UDP port x as a service (requires appropriate firewall permission)
// NULL => debug output goes to the bit bucket
// DEBUG => debug output goes to the debug window
// <anything else> => is assumed to be a file name
EXTERN int iDebugOutputDevice;
// iDebugOutputDevice is set by passwin to indicate the type of device tosend debug output to
// 0 => a normal output file (default)
// 1 => a serial port
// 2 => a console screen
// 3 => a TCPIP port
// 4 => the bit bucket
// 5 => the debug window
EXTERN char * lpDebugServerName;
// lpDebugServerName is the optional name for the debug output server. This argument has no meaning
// unless the "PORT" option is specified for iDebugOutputDevice.
// If lpDebugServerName is NULL (not specified) the default server name is localhost.
// If the value of lpDebugServerName starts with a number, it is assumed to be an IP4 address in dot
// notation.
// <anything else> => lpDebugServerName is assumed to be a DNS name.
EXTERN unsigned short iDebugServerPort INITIZERO;
// iDebugServerPort is the optional port number for the debug output server. This argument has no meaning
// unless the "PORT" option is specified for iDebugOutputDevice. If the port number is set to '0', then the
// default port number = 5001 is used.
EXTERN int ClearDebugScreenOnExit;
// option to clear the text from the debug screen. Note that this only works if the
// debug terminal is an ANSII terminal and the program terminates properly (won't clear on a crash)
// ClearDebugScreenOnExit == 0 => don't clear the screen (default)
// ClearDebugScreenOnExit == 1 or 3 => send ascii escape sequence ["[2J"] to clear the screen on exit
// ClearDebugScreenOnExit == 2 or 3 => send ascii escape sequence ["[2J"] to clear the screen at start
// ClearDebugScreenOnExit == anything else => don't clear the screen
#define BADUPRSTRCCALL		1
#define SUCCESS			0

// functions
int OpenTCPport(void);
int SetUpDebugConsole(void);
int IPSend(char * data, int MaxDataSize);
int ClosePort(void);
int OpenDebugDevice();
int CloseDebugDevice();
void DumpRamFile(char **lpHeapFile);
bool SaveStringInHeap(char **lpHeapFile,char *str);
bool StripCSI(char *ci, char *co);
int MsgBox(const char * message, ...);
int ErrorMessageTX(LPSTR msg, unsigned int mode);
int MessageBoxTX(const char * msg, const char * lpszBanner, unsigned int mode);
size_t _msize(void *memblock);
int tutPrintString (const char * szText);
void NewLine(void); 
void ScrollScreenBuffer(HANDLE, INT); 
int ReAllocateCharVector(unsigned char **p, int length);


#undef EXTERN
#undef INITIZERO
#undef INITSZERO
#undef INITBOOLFALSE
#undef INITBOOLTRUE
#undef INITNULL
#undef INITNEGDONE

#endif	// _DEBUG01_H
