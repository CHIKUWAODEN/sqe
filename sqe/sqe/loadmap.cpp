#include <set>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "loadmap.h"
#include "sqdlfcn.h"

/*---------------------------------------------------------------------------*/

/**
 * @class 個々のロード済みモジュールを表す構造体
 */
struct Entry
{
public:
	Entry (HSQUIRRELVM vm, SQChar *path, void *handle) {
		this->vm = vm;
		this->path = path;
		this->handle = handle;
	}
	
	HSQUIRRELVM vm;	// vm
	SQChar *path;	// パス
	void *handle;	// 拡張ライブラリ用
};

/** */
typedef std::set <Entry*> Entries;

/** */
static Entries s_entries;

/*---------------------------------------------------------------------------*/

static void
_sc_finalize_extend_lib (HSQUIRRELVM vm, void *handle)
{
	if( handle ) 
	{
		// 終了関数のシンボルを取得
		SQRESULT (*func) (HSQUIRRELVM) = (
			reinterpret_cast <SQRESULT (*) (HSQUIRRELVM)> (
				sc_dlsym (handle, _SC( "finalize" ))
			)
		);
		const SQChar* error = 0;
		if( (error = sc_dlerror ()) != NULL ) {
			scprintf (_SC( "%s\n" ), error);
		}
		else {
			// 初期化関数を実行
			if( SQ_FAILED( func (vm) ) ) {
				scprintf (_SC( "拡張ライブラリの終了に失敗しました\n" ));
			}
		}
		// 拡張ライブラリを終了する
		sc_dlclose (handle);
	}
}

/*---------------------------------------------------------------------------*/

bool
_sc_check_loaded_file (HSQUIRRELVM vm, const SQChar *path)
{
	for( Entries::iterator
	i  = s_entries.begin ();
	i != s_entries.end   (); i++
	){
		if( (*i)->vm == vm ) {
			if( ::strcmp ((*i)->path, path) == 0 ) {
				return true;
			}
		}
	}
	return false;
}

/*---------------------------------------------------------------------------*/

void
_sc_append_loaded_file_extend (HSQUIRRELVM vm, const SQChar *path, void *handle)
{
	SQChar *pathCopy = reinterpret_cast <SQChar *> (malloc (PATH_MAX));
	memset (pathCopy, '\0', PATH_MAX);
	if( pathCopy ) {
		scsprintf (pathCopy, "%s", path);
		s_entries.insert (new Entry (vm, pathCopy, handle));
	}
}

/*---------------------------------------------------------------------------*/

// handle を指定しないで _sc_append_loaded_file_extend を呼ぶことによって登録情報とする
void
_sc_append_loaded_file_nut (HSQUIRRELVM vm, const SQChar *path)  {
	_sc_append_loaded_file_extend (vm, path, 0);
}

/*---------------------------------------------------------------------------*/

void
_sc_clear_loaded_file (HSQUIRRELVM vm)
{
	for( Entries::iterator
	it  = s_entries.begin ();
	it != s_entries.end   (); it++
	){
		if( *it ) {
			// ハンドルから終了関数を呼ぶ
			if( (*it)->handle ) {
				_sc_finalize_extend_lib (vm, (*it)->handle);
			}
			free ((*it)->path);
			delete (*it);
		}
	}
	s_entries.clear ();
}
