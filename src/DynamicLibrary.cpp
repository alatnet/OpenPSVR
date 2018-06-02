#include "DynamicLibrary.h"

DynamicLibrary::DynamicLibrary(OPENPSVR_CSTR library){
	this->_loadOK = true;
	#ifdef OPENPSVR_WIN_PLATFORM
		this->lib_handle = LoadLibrary(library);
	#else
		this->lib_handle = dlopen(library, RTLD_LAZY);
	#endif
	if (this->lib_handle == NULL) this->_loadOK = false;
	#ifdef OPENPSVR_WIN_PLATFORM
		this->_error = GetLastError();
	#else
		this->_error = dlerror();
	#endif
}


DynamicLibrary::DynamicLibrary(OPENPSVR_CSTR library, OPENPSVR_DLL_FLAG_TYPE flags){
	this->_loadOK = true;
	#ifdef OPENPSVR_WIN_PLATFORM
		this->lib_handle = LoadLibraryEx(library, NULL,flags);
	#else
		this->lib_handle = dlopen(library, flags);
	#endif
	if (this->lib_handle == NULL) this->_loadOK = false;
	#ifdef OPENPSVR_WIN_PLATFORM
		this->_error = GetLastError();
	#else
		this->_error = dlerror();
	#endif
}

DynamicLibrary::~DynamicLibrary(void){
	if (!this->_loadOK) return;
	#ifdef OPENPSVR_WIN_PLATFORM
		FreeLibrary(this->lib_handle);
	#else
		dlclose(this->lib_handle);
	#endif
}

void* DynamicLibrary::getFunction(OPENPSVR_CSTR_A funcName){
	if (!this->_loadOK) return NULL;
	void* ret=NULL;

	#ifdef OPENPSVR_WIN_PLATFORM
		ret = GetProcAddress(this->lib_handle, funcName);
		if (ret==NULL) this->_error = GetLastError();
	#else
		dlerror();
		ret = dlsym(this->lib_handle, funcName);
		this->_error = dlerror();
	#endif

	return ret;
}


#ifdef OPENPSVR_WIN_PLATFORM
	HGLOBAL DynamicLibrary::getResource(OPENPSVR_CSTR_WIN32 name, OPENPSVR_CSTR_WIN32 type){
		if (!this->_loadOK) return NULL;
		HRSRC _hRSrc = FindResource(this->lib_handle, name, type);
		if (_hRSrc == NULL){
			this->_error = GetLastError();
			return NULL;
		}

		HGLOBAL ret=NULL;
		ret = LoadResource(this->lib_handle,_hRSrc);
		if (ret == NULL){
			this->_error = GetLastError();
		}
		return ret;
	}
#endif
