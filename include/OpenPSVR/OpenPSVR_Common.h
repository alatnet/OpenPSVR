#pragma once

#ifndef _COMMON_HPP
#define	_COMMON_HPP

//check which platform we are compiling to.
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64)
    #define OPENPSVR_WIN_PLATFORM
#elif defined(__APPLE__) || defined(__APPLE_CC__)
    #define OPENPSVR_APPLE_PLATFORM
#elif defined(__GNUC__) && (!(defined(__WIN32__) || defined(_WIN32)) && !(defined(__APPLE__) || defined(__APPLE_CC__)))
    #define OPENPSVR_NIX_PLATFORM
#else
    #error No Recognized Platform!
#endif

//see if we are using propriotary compilers.
#ifdef _MSC_VER
    #define OPENPSVR_VISUAL_C
#elif defined(__BORLANDC__)
    #define OPENPSVR_BORLANDC
#elif defined(__GNUC__) && (defined(__WIN32__) || defined(_WIN32))
    #define OPENPSVR_MINGW
    #define OPENPSVR_GNUC
#elif defined(__GNUC__)
    #define OPENPSVR_GNUC
#endif

//check if we are compiling to 32 or 64 bit.
#if defined(__x86_64__) || defined(_M_X64) || defined(_WIN64)
    #define OPENPSVR_ARCH_64
#else
    #define OPENPSVR_ARCH_32
#endif

//see if we are compiling with unicode support.
#if defined(_UNICODE) || defined(UNICODE)
	#define OPENPSVR_UNICODE
#endif

#if defined(OPENPSVR_BUILD_DLL) || defined (OPENPSVR_BUILD_SO) //set exports for building a dynamic library
    #ifdef OPENPSVR_WIN_PLATFORM
        #if defined(OPENPSVR_MINGW) || defined(OPENPSVR_VISUAL_C)
            #define OPENPSVR_DLL_DECLARE __declspec(dllexport)
            #define OPENPSVR_DLL_EXTERN extern
            #define OPENPSVR_DLL_C_EXTERN extern "C"
        #else
            #define OPENPSVR_DLL_DECLARE __attribute__ ((dllexport))
            #define OPENPSVR_DLL_EXTERN extern
            #define OPENPSVR_DLL_C_EXTERN extern "C"
        #endif
    #else
        #ifdef OPENPSVR_USE_DECLSPEC
            #define OPENPSVR_DLL_DECLARE __declspec(dllexport)
            #define OPENPSVR_DLL_EXTERN extern
            #define OPENPSVR_DLL_C_EXTERN extern "C"
        #else
            #if defined __GNUC__ && __GNUC__ >= 4
                #define OPENPSVR_DLL_DECLARE __attribute__ ((visibility ("default")))
            #else
                #define OPENPSVR_DLL_DECLARE __attribute__ ((dllexport))
            #endif
            #define OPENPSVR_DLL_EXTERN extern
            #define OPENPSVR_DLL_C_EXTERN extern "C"
        #endif
    #endif
#elif defined (OPENPSVR_IMPORT_DLL) || defined (OPENPSVR_IMPORT_SO) //set exports for importing a dynamic library
    #ifdef OPENPSVR_WIN_PLATFORM
        #if defined(OPENPSVR_MINGW) || defined(OPENPSVR_VISUAL_C)
            #define OPENPSVR_DLL_DECLARE __declspec(dllimport)
            #define OPENPSVR_DLL_EXTERN
            #define OPENPSVR_DLL_C_EXTERN
        #else
            #define OPENPSVR_DLL_DECLARE __attribute__ ((dllimport))
            #define OPENPSVR_DLL_EXTERN
            #define OPENPSVR_DLL_C_EXTERN
        #endif
    #else
        #ifdef OPENPSVR_USE_DECLSPEC
            #define OPENPSVR_DLL_DECLARE __declspec(dllimport)
            #define OPENPSVR_DLL_EXTERN
            #define OPENPSVR_DLL_C_EXTERN
        #else
            #if defined __GNUC__ && __GNUC__ >= 4
                #define OPENPSVR_DLL_DECLARE __attribute__ ((visibility ("default")))
            #else
                #define OPENPSVR_DLL_DECLARE __attribute__ ((dllimport))
            #endif
            #define OPENPSVR_DLL_EXTERN
            #define OPENPSVR_DLL_C_EXTERN
        #endif
    #endif
#else	//don't set exports, compile to a .lib file
    #define OPENPSVR_DLL_DECLARE
    #define OPENPSVR_DLL_EXTERN
    #define OPENPSVR_DLL_C_EXTERN
#endif

#ifdef OPENPSVR_UNICODE
	typedef wchar_t OPENPSVR_CHAR;
	typedef wchar_t* OPENPSVR_CSTR;
	typedef char OPENPSVR_CHAR_A;
	typedef char* OPENPSVR_CSTR_A;
	#ifdef OPENPSVR_WIN_PLATFORM
		#ifdef _INC_WINDOWS
			typedef LPCWSTR OPENPSVR_CSTR_WIN32;
			typedef LPCSTR OPENPSVR_CSTR_WIN32_A;
		#else
			typedef wchar_t* OPENPSVR_CSTR_WIN32;
			typedef char* OPENPSVR_CSTR_WIN32_A;
		#endif
	#else
		typedef wchar_t* OPENPSVR_CSTR_WIN32;
		typedef char* OPENPSVR_CSTR_WIN32_A;
	#endif
#else
	typedef char OPENPSVR_CHAR;
	typedef char* OPENPSVR_CSTR;
	typedef char OPENPSVR_CHAR_A;
	typedef char* OPENPSVR_CSTR_A;
	#ifdef OPENPSVR_WIN_PLATFORM
		#ifdef _INC_WINDOWS
			typedef LPCSTR OPENPSVR_CSTR_WIN32;
			typedef LPCSTR OPENPSVR_CSTR_WIN32_A;
		#else
			typedef char* OPENPSVR_CSTR_WIN32;
			typedef char* OPENPSVR_CSTR_WIN32_A;
		#endif
	#else
		typedef char* OPENPSVR_CSTR_WIN32;
		typedef char* OPENPSVR_CSTR_WIN32_A;
	#endif
#endif

#endif	/* _COMMON_HPP */