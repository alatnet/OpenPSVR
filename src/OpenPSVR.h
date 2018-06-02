#pragma once

#include <OpenPSVR/OpenPSVR_Common.h>

/*#if defined(_WIN32)
	#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
	#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
	#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
	#define HMD_DLL_IMPORT extern "C" 
#else
	#error "Unsupported Platform."
#endif
*/

OPENPSVR_DLL_C_EXTERN OPENPSVR_DLL_DECLARE void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode);