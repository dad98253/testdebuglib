/*
 * isPowerOfTwo.cpp
 *
 *  Created on: May 15, 2018
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





// ref: http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
// (also decribed in Hacker's Delight)

// 32 bit version :

int isPowerOfTwo (unsigned int x)
{
  return ((x != 0) && ((x & (~x + 1)) == x));
}

// 64 bit version :

#ifdef WINDOZE
int isPowerOfTwo64 (unsigned __int64 x)
{
  return ((x != 0x0ui64) && ((x & (~x + 0x1ui64)) == x));
}
#else
int isPowerOfTwo64 (unsigned long long int x)
{
  return ((x != 0ULL) && ((x & (~x + 1ULL)) == x));
}
#endif
