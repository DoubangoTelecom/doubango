/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke, Lucent Technologies
   All rights reserved.
*/

/* WARNING: Make sure that the proper flags are defined for your system */

/*
   Types definitions
*/

#if defined(__BORLANDC__) || defined (__WATCOMC__) || defined(_MSC_VER) || defined(__ZTC__) || defined(__HIGHC__)
typedef  long  int   Word32   ;
typedef  short int   Word16   ;
typedef  short int   Flag  ;
#elif defined( __sun)
typedef short  Word16;
typedef long  Word32;
typedef int   Flag;
#elif defined(__unix__) || defined(__unix)
typedef short Word16;
typedef int   Word32;
typedef int   Flag;
#elif defined(VMS) || defined(__VMS)
typedef short  Word16;
typedef long  Word32;
typedef int   Flag;
#else
#error  COMPILER NOT TESTED typedef.h needs to be updated, see readme
#endif



#if defined(_MSC_VER)
#	define G729_INLINE	__forceinline
#elif defined(__GNUC__) && !defined(__APPLE__)
#	define G729_INLINE	__inline
#else
#	define G729_INLINE	
#endif

#if ANDROID || TARGET_OS_EMBEDDED || defined(_WIN32_WCE)
#	define G729_ARM 1
#else
#	define G729_ARM 0
#endif