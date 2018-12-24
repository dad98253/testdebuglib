/*
 * OpenTCPport.cpp
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

/*
 * I did some work like this, through much reading of MSDN and the sockets documentation I got a list of differences between Winsocks and POSIX sockets, many are minor, sign differences or type differences.


uses closesocket() to close a socket, instead of close().
uses an int for the third parameter in bind().
uses an int* for the third parameter in accept().
defines a SOCKET as unsigned int.
socket() returns INVALID_SOCKET(unsigned int) on error, *nix returns -1.
connect() returns SOCKET_ERROR(int) on error, *nix returns -1.
bind() returns SOCKET_ERROR(int) on error, *nix returns -1.
listen() returns SOCKET_ERROR(int) on error, *nix returns -1.
accept() returns INVALID_SOCKET(unsigned int) on error, *nix returns -1.
select() returns SOCKET_ERROR(int) on error, *nix returns -1.
send() returns SOCKET_ERROR(int) on error, *nix returns -1.
send() uses an int for the third parameter (length).
recv() returns SOCKET_ERROR(int) on error, *nix returns -1.
setsockopt() returns SOCKET_ERROR(int) on error, *nix returns -1.
uses an const char* for the fourth parameter in setsockopt().
ioctlsocket() instead of fcntl(), this returns SOCK_ERROR(int), *nix returns -1.
ioctlsocket() has the option as a u_long, fcntl() uses an int.

Not mentioned, select() on Linux modifies the timeout value to reflect time not slept.
 */


int OpenTCPport(void) {
	char *default_server_name = (char*)"localhost";
	if ( lpDebugServerName == NULL ) {
		server_name = default_server_name;
	} else {
		server_name = lpDebugServerName;
	}
//fprintf(stderr,"opening server %s for port %i\n",server_name,iDebugServerPort);
#ifdef WINDOZE
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) {
		fprintf(stderr,"WSAStartup failed with error %d\n",WSAGetLastError());
		WSACleanup();
		return 0;
	}
#endif
	//
	// Attempt to detect if we should call gethostbyname() or
	// gethostbyaddr()
	if (isalpha(server_name[0])) {   /* server address is a name */
		hp = gethostbyname(server_name);
	}
	else  { /* Convert nnn.nnn address to a usable one */
		addr = inet_addr(server_name);
		hp = gethostbyaddr((char *)&addr,4,AF_INET);
	}
	if (hp == NULL ) {
#ifdef WINDOZE
#ifdef DEBUGOPENTCPPORT
		fprintf(stderr,"Client: Cannot resolve address [%s]: Error %d\n",
			server_name,WSAGetLastError());
#endif	// DEBUGOPENTCPPORT
		WSACleanup();
		return 0;
#else	// WINDOZE
#ifdef DEBUGOPENTCPPORT
		int errsv = errno;
		int herrsv = h_errno;
		fprintf(stderr,"Client: Cannot resolve address [%s]: Error %d, h_err = %i\n",
			server_name,errsv,herrsv);
		fprintf(stderr,"%s\n", hstrerror(h_errno));
		if (h_errno == HOST_NOT_FOUND) fprintf(stderr,"Note: on linux, ip addresses must have valid RDNS entries\n");
#endif	// DEBUGOPENTCPPORT
#endif	// WINDOZE
//		return 0;	////////////   ???? why is this commented out on linux??
	}

	//
	// Copy the resolved information into the sockaddr_in structure
	//  iDebugServerPort
	memset(&server,0,sizeof(server));
	if ( hp == NULL ){
		server.sin_addr.s_addr = inet_addr(server_name);
		server.sin_family = AF_INET;
	} else {
		memcpy(&(server.sin_addr),hp->h_addr,hp->h_length);
		server.sin_family = hp->h_addrtype;
	}
	if ( iDebugServerPort != 0 ) port = iDebugServerPort;
	server.sin_port = htons(port);

	conn_socket = socket(AF_INET,socket_type,0); /* Open a socket */
	if (conn_socket <0 ) {
#ifdef WINDOZE
		fprintf(stderr,"Client: Error Opening socket: Error %d\n",
			WSAGetLastError());
		WSACleanup();
#else
		int errsv = errno;
		fprintf(stderr,"Client: Error Opening socket: Error %d\n",
				errsv);
#endif
		return 0;
	}

	//
	// Notice that nothing in this code is specific to whether we 
	// are using UDP or TCP.
	// We achieve this by using a simple trick.
	//    When connect() is called on a datagram socket, it does not 
	//    actually establish the connection as a stream (TCP) socket
	//    would. Instead, TCP/IP establishes the remote half of the
	//    ( LocalIPAddress, LocalPort, RemoteIP, RemotePort) mapping.
	//    This enables us to use send() and recv() on datagram sockets,
	//    instead of recvfrom() and sendto()


	if ( hp != NULL ) {
//		dfprintf(__LINE__,__FILE__,DEBUGOPENTCPPORT,"Client connecting to: %s\n",hp->h_name); // warning: can cause a loop !
	} else {
//		dfprintf(__LINE__,__FILE__,DEBUGOPENTCPPORT,"Client connecting to: %s\n",server_name); // warning: can cause a loop !
	}

	if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))
		== SOCKET_ERROR) {
#ifdef WINDOZE
		fprintf(stderr,"connect() failed: %d\n",WSAGetLastError());
		WSACleanup();
#else
		int errsv = errno;
		fprintf(stderr,"connect() failed: %d\n",errsv);
#endif
		return 0;
	}

	return 1;
}
