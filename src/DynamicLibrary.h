#pragma once
#ifndef _DYNAMIC_LIBRARY_HPP_
#define	_DYNAMIC_LIBRARY_HPP_

#include <OpenPSVR/OpenPSVR_Common.h>

#ifdef OPENPSVR_WIN_PLATFORM
	#include <Windows.h>
	typedef DWORD OPENPSVR_DLL_FLAG_TYPE;
	typedef DWORD OPENPSVR_DLL_ERROR;
	#define LIBHANDLE HINSTANCE
#else
	#include <dlfcn.h>
	typedef int OPENPSVR_DLL_FLAG_TYPE;
	typedef char* OPENPSVR_DLL_ERROR;
	#define LIBHANDLE void *
#endif

class DynamicLibrary {
public:
	DynamicLibrary(OPENPSVR_CSTR library);
	DynamicLibrary(OPENPSVR_CSTR library, OPENPSVR_DLL_FLAG_TYPE flags);
	virtual ~DynamicLibrary(void);

	bool loadOK() { return this->_loadOK; };
	OPENPSVR_DLL_ERROR getError() { return this->_error; };

	void* getFunction(OPENPSVR_CSTR_A funcName);

	#ifdef OPENPSVR_WIN_PLATFORM
		HGLOBAL getResource(OPENPSVR_CSTR_WIN32 name, OPENPSVR_CSTR_WIN32 type);
	#endif
private:
	LIBHANDLE lib_handle;

	bool _loadOK;
	OPENPSVR_DLL_ERROR _error;
};

#endif
