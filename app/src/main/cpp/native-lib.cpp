#include <jni.h>
#include <string>

#ifdef __clang__
#include <android/log.h>
#endif


#ifndef WINDOZE
#include "lindows.h"
#include <strings.h>
#define _stricmp	strcasecmp
#endif	// WINDOZE

#define NEED_OS_FORK
#undef _GNU_SOURCE
#define _GNU_SOURCE 1 /* for strcasestr in legacy builds */

#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "debug01.h"


#ifdef WINDOZE
int isPowerOfTwo64 (unsigned __int64 x);
#else
int isPowerOfTwo64 (unsigned long long x);
#endif


char *szProgFilename = NULL;
BOOL bStdioRedirectedToConsole;

DEBUGOPTIONSTYPE myoptions;
DEBUGOPTIONSTYPE* pmyoptions = NULL;


#define TESTLIB "testdebug"
#ifdef __clang__
#define PRINTSTERR  __android_log_print(ANDROID_LOG_DEBUG, TESTLIB,
#define PRINTSTDOUT  __android_log_print(ANDROID_LOG_DEBUG, TESTLIB,
#define ADDCR
#else   // clang
#define PRINTSTERR fprintf(stderr,
#define PRINTSTDOUT printf(
#define ADDCR "\n"
#endif  // clang


extern "C" JNIEXPORT jstring JNICALL
Java_net_kuras_1sea_testdebuglib_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {


#ifdef DEBUG
    PRINTSTDOUT "   DEBUG is set\n");
#else	// DEBUG
    PRINTSTDOUT "   DEBUG is not set\n");
#endif	// DEBUG

    int errflag = 1;
    pmyoptions = &myoptions;
    int retval = 0;

    long long unsigned int lluRelAddress = 0x228;
    const char * szFunctionName = "main";
    char * szExeFile;

    if( ( retval = debug_init ((void**)&pmyoptions, 1 )) ) {
        PRINTSTDOUT "bad debug init\n");
        goto myerror;
    }


    szProgFilename = (char *)"main";



    dfprintf(__LINE__,__FILE__,TRACE,"calling load_debug...\n");
//    load_debug ();   need to add this to set debug flags...
    debugInitialVirbosity = debugVirbosity = 1;
    iDebugOutputDevice = 3;
//    lpDebugServerName = (char *)"192.168.1.34";
    lpDebugServerName = (char *)"grandma";
    ColorDebug = 1;
    if ( set_debug_device((char *)"TCPPORT") ) {
        PRINTSTERR "set_debug_device failed" ADDCR);
        goto myerror;
    } else {
        dfprintf(__LINE__, __FILE__, DEBUGINIT,
                 "\33[1;32mDebug output device set to type %i\33[0m (%s)\n", iDebugOutputDevice,
                 debug_devices[iDebugOutputDevice]);
    }

    dfprintf(__LINE__,__FILE__,TRACE,"returned from load_debug...\n");

/*
	if( ( retval = set_debug_device ((char*)"RAM") ) ) {
		printf("bad debug device type\n");
		goto myerror;
	}
*/


myexit:

    debug_close ();

    errflag = 0;
myerror:
    std::string hello;
    if (errflag) {
        hello = "error";
    } else {
        hello = "Hello from debug test";
    }
    return env->NewStringUTF(hello.c_str());
}




