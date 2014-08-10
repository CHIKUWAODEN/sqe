#ifndef ___INCLUDED_LIBSC_H___
#define ___INCLUDED_LIBSC_H___


#include <squirrel.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * require 関数を使うための初期化処理を行う.
 *
 * @param vm 対象となる VM
 * @return void
 */
extern void
sc_register_sclib (HSQUIRRELVM vm);


/**
 * require 関数の機能に関する終了処理を行う.
 *
 * @param vm 対象となる VM（通常 register_sclib に渡したものを渡す）
 * @return void
 */
extern void
sc_finalize_extend_libs (HSQUIRRELVM vm);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // ___INCLUDED_LIBSC_H___
