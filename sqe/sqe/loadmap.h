#ifndef ___INCLUDED_LOADMAP_H___
#define ___INCLUDED_LOADMAP_H___


#include <squirrel.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * ロード済みかどうかチェックする.
 *
 * @param vm 対象となる VM
 * @param path モジュールのパス
 */
extern bool
_sc_check_loaded_file (HSQUIRRELVM vm, const SQChar *path);


/**
 * ロード済みな SEL（動的ライブラリ 形式）の情報を追加する.
 *
 * @param vm 対象となる VM
 * @param path 動的ライブラリファイルのパス
 * @param path 動的ライブラリファイルのハンドル
 * @return void
 */
extern void
_sc_append_loaded_file_extend (HSQUIRRELVM vm, const SQChar *path, void *handle);


/**
 * ロード済みな SEL（.nut 形式）の情報を追加する.
 *
 * @param vm 対象となる VM
 * @param path .nut ファイルのパス
 * @return void
 */
extern void
_sc_append_loaded_file_nut (HSQUIRRELVM vm, const SQChar *path);


/**
 * ロード済みファイル情報をクリアする. 
 *
 * @param vm 対象となる VM
 * @return void
 */
extern void
_sc_clear_loaded_file (HSQUIRRELVM vm);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // ___INCLUDED_LOADMAP_H___
