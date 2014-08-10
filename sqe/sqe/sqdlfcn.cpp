#include <stdio.h>
#include <dlfcn.h>
#include <squirrel.h>
#include "sqdlfcn.h"

// TODO: linux 以外のプラットフォームへの対応

/*---------------------------------------------------------------------------*/

void*
sc_dlopen (SQChar *loadpath) 
{
	return dlopen (
		loadpath, 
	//	RTLD_LAZY
		RTLD_NOW
	);
}

/*---------------------------------------------------------------------------*/

const SQChar*
sc_dlerror (void) {
	return dlerror ();
}

/*---------------------------------------------------------------------------*/

// 拡張ライブラリのシンボルを取得
void *sc_dlsym (void *handle, const SQChar *symbol) {
	return dlsym (handle, symbol);
}

/*---------------------------------------------------------------------------*/

// 拡張ライブラリをクローズする
int sc_dlclose (void *handle) {
	return dlclose (handle);
}
