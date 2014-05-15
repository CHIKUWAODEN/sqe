/* C/C++ standard libs */
#include <stdio.h>

/* Squirrel API and standaed libs, libsc */
#include <squirrel.h>
#include <sqstdio.h>

#include "libsc.h"
#include "loadmap.h"
#include "loadpath.h"
#include "sqdlfcn.h"

/*---------------------------------------------------------------------------*/

/**
 * .nut ファイル用 の require 実装.
 *
 * @param vm 対象となる　VM.
 * @param path .nut ファイルをフルパスで渡す.
 * @return 処理の成否を返す.
 */
static SQRESULT
_sc_import_nut (HSQUIRRELVM vm, SQChar *path)
{
	// スクリプトファイルの読み込み
	if( SQ_FAILED( sqstd_loadfile (vm, path, SQTrue) ) ) { 
		return SQ_ERROR;
	}

	// sq_call 関数の設定
	sq_push (vm, -2);
	const SQRESULT callResult = sq_call (
		vm, 1,
		SQFalse,
		SQTrue
	);
	if( SQ_FAILED( callResult ) ) {
		const SQChar *error = 0;
		sq_getlasterror (vm);
		if( SQ_SUCCEEDED( sq_getstring (vm, -1, &error) ) ) {
			scprintf (_SC( "libsc ERROR : [%s]\n" ), error);
		}
		return SQ_ERROR;
	} 
	// 読み込みに成功しているのでロード済みファイルとして追加
	_sc_append_loaded_file_nut (vm, path);
	return SQ_OK;
}

/*---------------------------------------------------------------------------*/

/**
 * 動的ロードライブラリ用 の require 実装.
 *
 * @param vm 対象となる VM.
 * @param path 動的ロードライブラリファイルのパス.
 * @return 処理の成否を返す.
 */
static SQRESULT 
_sc_import_extend (HSQUIRRELVM vm, const SQChar *path) 
{
	// ライブラリの動的ローディング
	for(;;) {
		void *handle = sc_dlopen (const_cast <SQChar*> (path));
		if( !handle ) {
			scprintf (_SC( "%s\n" ), sc_dlerror ());
			break;
		} 
		
		// 初期化関数のシンボルを取得
		SQRESULT (*func) (HSQUIRRELVM) = (
			reinterpret_cast <SQRESULT (*) (HSQUIRRELVM)> (
				sc_dlsym (handle, _SC( "initialize" ))
			)
		);
		const SQChar* error = 0;
		if( (error = sc_dlerror ()) != NULL ) {
			scprintf (_SC( "%s\n" ), error);
			break;
		}
		else {
			// 初期化関数を実行
			if( SQ_FAILED( func (vm) ) ) {
				scprintf (_SC( "拡張ライブラリの初期化に失敗しました\n" ));
			}
		}

		// 読み込み済みファイルとして追加
		_sc_append_loaded_file_extend (vm, path, handle);
		break;
	}

	return SQ_ERROR;
}

/*---------------------------------------------------------------------------*/

static SQInteger
_sc_require (HSQUIRRELVM vm)
{
	// 引数としてテーブルが渡されている場合はそれを取り出し、そうでなければ予め RootTable を Push しておく
    SQInteger args = sq_gettop (vm);
    switch( args ) {
    case 2:
        sq_pushroottable (vm);
        break;
    default:
        break;
    }

	for(;;) // breakable
	{
		// モジュールの探索キーを取得
		const SQChar *key = 0;
		if( SQ_FAILED( sq_getstring (vm, -2, &key) ) ) { 
			break;
		}

		// スタック上のテーブルオブジェクトを取得
		if( sq_gettype (vm, -1 ) != OT_TABLE ) {
			scprintf (_SC( "argument is not TABLE\n" ));
			break;
		}
		HSQOBJECT table;
		sq_getstackobj (vm, -1, &table);
		sq_addref (vm, &table);

		// スタックをクリアし、ターゲットテーブルをスタックに Push
		sq_settop (vm, 0);
		sq_pushobject (vm, table);

		// 補完されたパスを生成しつつ順番に調べていく
		SQChar *pathComplete = 0;
		SCImportPathGenerator *generator = SCImportPathGenerator::createInstance ();
		if( 0 == generator ) {
			break;
		}
		// 生成された補完済みパスがすでに読み込まれているのかチェックしつつ、
		// 生成済みでないならそれをロードする
		generator->set (key);
		while( (pathComplete = generator->get ()) )
		{
			if( _sc_check_loaded_file (vm, pathComplete) ) {
				break;
			}
			else
			if( generator->getEntryType () == SC_ENTRY_TYPE_NUT ) {
				if( SQ_SUCCEEDED( _sc_import_nut (vm, pathComplete) ) ) {
					break;
				}
			}
			else
			if( generator->getEntryType () == SC_ENTRY_TYPE_EXTEND ) {
				if( SQ_SUCCEEDED( _sc_import_extend (vm, pathComplete) ) ) {
					break;
				}
			}
		}
		// 補完パスの生成器の終了
		if( generator ) {
			SCImportPathGenerator::deleteInstance (generator);
		}

		sq_settop (vm, 0);
		sq_pushobject (vm, table);
		break;
	}

	// require はテーブルオブジェクトを返す
	return 1;
}

/*---------------------------------------------------------------------------*/

void sc_register_sclib (HSQUIRRELVM vm)
{
	sq_pushstring (vm, _SC( "require" ), -1);
	sq_newclosure (vm, _sc_require, 0);
	sq_setnativeclosurename (vm, -1, _SC( "_sc_require" ));
	if( SQ_FAILED( sq_newslot (vm, -3, SQFalse) ) ) {
		sq_getlasterror (vm);
		const SQChar *error = 0;
		if( SQ_SUCCEEDED( sq_getstring (vm, -1, &error) ) ) {
			scprintf (_SC( "Error [%s]\n" ), error);
		}
	}
}

/*---------------------------------------------------------------------------*/

// 拡張ライブラリのアンロード処理などを行う
void
sc_finalize_extend_libs (HSQUIRRELVM vm) {
	_sc_clear_loaded_file (vm);
}
