#ifndef ___INICLUDED_LOADPATH_H___
#define ___INICLUDED_LOADPATH_H___


#include <set>
#include <list>
#include <squirrel.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * loadpath モジュールの初期化を行う.
 *
 * @return void
 */
extern void sc_initialize_loadpath ();

/**
 * require が探索するロードパスの登録.
 *
 * @param path パス文字列.
 * @return void
 */
extern void sc_register_loadpath (const SQChar *path);

// ロードパスを格納するためのコンテナ
typedef std::set <SQChar *> Paths;

/*---------------------------------------------------------------------------*/

/**
 * 拡張ライブラリの種別.
 */
enum ScEntryType
{
	SC_ENTRY_TYPE_NUT     = 0x00000000,
	SC_ENTRY_TYPE_EXTEND  = 0x00000001,
	SC_ENTRY_TYPE_INVALID = 0xFFFFFFFF 
};

/*---------------------------------------------------------------------------*/

/**
 * @class import 用のパスを繰り返して取り出す処理を行うためのクラス
 */
class SCImportPathGenerator
{
private: 
	 SCImportPathGenerator ();
	~SCImportPathGenerator ();

public:
	/**
	 * このクラスの singleton なインスタンスを生成する.
	 * @return SCImportPathGenerator クラスのインスタンスを返す.
	 */
	static SCImportPathGenerator *createInstance ();

	/**
	 * このクラスの singleton なインスタンスを破棄する.
	 * @return なし.
	 */
	static void deleteInstance (SCImportPathGenerator* instance);

public:
	/**
	 * モジュールパスから実際にファイルシステムからの読み込みを行うための
	 * 検索候補となるファイルパス達を生成する.
	 *
	 * @param pathImport モジュールパス.
	 * @return void
	 */
	void set (const SQChar *pathImport);

	/**
	 * set であらかじめ生成しておいたものを順番にかえす.
	 * かえせるものがなくなったら NULL をかえす.
	 * 
	 * @param void
	 * @return set であらかじめ生成しておいたものを順番にかえす.
	 * かえせるものがなくなったら NULL をかえす.
	 */
	SQChar *get (void);
	ScEntryType getEntryType (void);

private:
	ScEntryType getCurrentEntryType (const SQChar* path);

private:
	void clear (void);

private:
	typedef std::list <SQChar *> PathList;
	typedef std::list <SQChar *>::iterator PathIterator;
	PathList pathList;
	PathIterator pathIterator;

private:
	static SCImportPathGenerator *currentInstance;
	ScEntryType saveLastEntryType;
};


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // ___INICLUDED_LOADPATH_H___
