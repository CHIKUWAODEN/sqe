/* C++ Standard */
#include <string>
#include <list>

/* C Standard */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>

/* Squirrel Embedding API */
#include <squirrel.h>

/* Sciurus */
#include "loadpath.h"


static Paths s_paths;

/*---------------------------------------------------------------------------*/

// TODO: これも環境依存のため、外部から動的に設定できるようにする必要がある
static SQChar*
sc_realpath (const SQChar *path, SQChar *resolvedPath) 
{
	/* Linux */
	SQChar *realPath = realpath (path, resolvedPath);
	if( realPath != resolvedPath ) {
		/*
		switch( errno ) 
		{
			// パスのディレクトリ部分に、読み出し許可または検索許可が与えられていない。 
			case EACCES:
				scprintf ("realpath error : EACCES (%s)\n", path);
				break;

			// pathか resolved_pathのいずれかが NULL である。 
			// (libc5 では、このような場合 segfault を起こすだけであろう) 但し、下記の「注意」の節を参照のこと。 
			case EINVAL:
				scprintf ("realpath error : EINVAL (%s)\n", path);
				break;

			// ファイルシステムを読むときに、I/Oエラーが起こった。 
			case EIO:
				scprintf ("realpath error : EIO (%s)\n", path);
				break;

			// パス名の変換にあたり、解決すべきシンボリック・リンクの数が多過ぎた。 
			case ELOOP:
				scprintf ("realpath error : ELOOP (%s)\n", path);
				break;

			// パス名の一要素の文字数が NAME_MAXを越えている、またはパス名全体の文字数が PATH_MAXを越えている。 
			case ENAMETOOLONG:
				scprintf ("realpath error : ENAMETOOLONG (%s)\n", path);
				break;

			// 指定されたファイルが存在しない。 
			case ENOENT:
				scprintf ("realpath error : ENOENT (%s)\n", path);
				break;

			// パスのディレクトリ要素が、ディレクトリでない。 
			case ENOTDIR:
				scprintf ("realpath error : ENOTDIR (%s)\n", path);
				break;
		}
		//*/
		return 0;
	}
	return realPath;
}

/*---------------------------------------------------------------------------*/

/**
 *
 * @param path 
 * @param out
 * @return true  success
 * @return false error
 */
static bool
_sc_resolve_path (const SQChar *path, SQChar *out) 
{
	typedef std::basic_string <SQChar> SQString;
	SQChar realPath[ PATH_MAX ] = "";
	if( sc_realpath (path, realPath) == realPath) 
	{
		const SQChar *homePath = getenv ("HOME");
		SQString resolvedPath (realPath);
		SQString::size_type pos = 0;
		while( 
		pos  = resolvedPath.find (_SC( "~" ), pos),
		pos != SQString::npos
		){
			resolvedPath.replace (pos, 1, homePath);
		}
		scsprintf (out, _SC( "%s" ), resolvedPath.c_str ());
		return true;
	}
	return false;
}

/*---------------------------------------------------------------------------*/

/**
 * 環境変数を取得する関数のラッパー関数.
 * 現状 Linux 用となっている.
 *
 * @param env 環境変数名.
 * @return 環境変数の値.
 */
static
SQChar *_sc_getenv (const SQChar *env) {
	return getenv (env);
}


/**
 * SEL のロードパスリストを初期化する。
 * 現状 Linux 用となっている.
 *
 * @return void
 */
void 
sc_initialize_loadpath ()
{
	// 登録パスをいちどすべてクリア
	for( Paths::iterator
	path  = s_paths.begin ();
	path != s_paths.end   (); path++ ) { 
		if( *path ) { 
			free (*path); 
		}
	}
	s_paths.clear ();


	// デフォルトの登録パス
	sc_register_loadpath (_SC( "." ));
	sc_register_loadpath (_SC( "/usr/lib/sqe/sel" ));
	{
		SQChar temp[ PATH_MAX ];
		memset (temp, 0, sizeof(temp));
		scsprintf (temp, _SC( "%s/.sqe/sel" ), _sc_getenv (_SC( "HOME" )));
		sc_register_loadpath (temp);
	}

	// 環境変数からの入力
	char *envPath = _sc_getenv ("SQE_SEL_PATH");
	char *token   = strtok (envPath, ":");
	while( token ) {
		if( token ) {
			sc_register_loadpath (token);
		}
		token = strtok (NULL, ":");
	}
}

/*---------------------------------------------------------------------------*/

// パス文字列をコピーして、コピーした文字列をコンテナに格納する.
// FIXME PATH_MAX 分常に malloc するのはもったいない.
void sc_register_loadpath (const SQChar *path)
{
	if( path ) {
		SQChar *pathCopy = reinterpret_cast <SQChar *> (malloc (PATH_MAX));
		if( pathCopy ) {
			strcpy (pathCopy, path);
			s_paths.insert (pathCopy);
		}
	}
}

/*---------------------------------------------------------------------------*/

SCImportPathGenerator*
SCImportPathGenerator::currentInstance = 0;


SCImportPathGenerator::SCImportPathGenerator ()
	: saveLastEntryType (SC_ENTRY_TYPE_INVALID)
{}


SCImportPathGenerator::~SCImportPathGenerator () {
	this->clear ();
}


SCImportPathGenerator*
SCImportPathGenerator::createInstance ()
{
	if( 0 == SCImportPathGenerator::currentInstance ) {
		SCImportPathGenerator::currentInstance = new SCImportPathGenerator ();
	}
	return SCImportPathGenerator::currentInstance;
}


void 
SCImportPathGenerator::deleteInstance (SCImportPathGenerator* instance)
{
	if( SCImportPathGenerator::currentInstance == instance ) {
		delete
		SCImportPathGenerator::currentInstance;
		SCImportPathGenerator::currentInstance = 0;
	}
}


// TODO: コンパイル済みのものがあれば優先的にそれを入力する
void
SCImportPathGenerator::set (const SQChar *pathImport)
{
	// 既に確保されているものがあれば解放しておく
	this->clear ();

	// .nut 拡張子を持つファイルパスを生成する
	for( Paths::iterator
	path  = s_paths.begin ();
	path != s_paths.end   (); path++ 
	){
		SQChar *pathBase = (*(path));
		SQChar buffer[ PATH_MAX ] = "";
		scsprintf (buffer, _SC( "%s/%s.nut" ), pathBase, pathImport);

		SQChar resolvedPath[ PATH_MAX ] = "";
		if( _sc_resolve_path (buffer, resolvedPath) )
		{
			SQChar *newPath= reinterpret_cast <SQChar *> (malloc (PATH_MAX));
			if( newPath ) {
				memset (newPath, '\0', PATH_MAX);
				strcpy (newPath, resolvedPath);
				this->pathList.push_back (newPath);
			}
		}
	}

	// lib{モジュール名}.so 形式のファイルパスを生成する
	for( Paths::iterator
	path  = s_paths.begin ();
	path != s_paths.end   (); path++ 
	){
		SQChar *pathBase = *path;
		SQChar buffer[ PATH_MAX ] = "";
		scsprintf (buffer, _SC( "%s/lib%s.so" ), pathBase, pathImport);

		SQChar resolvedPath[ PATH_MAX ] = "";
		if( _sc_resolve_path (buffer, resolvedPath) )
		{
			SQChar *newPath= reinterpret_cast <SQChar *> (malloc (PATH_MAX));
			if( newPath ) {
				memset (newPath, '\0', PATH_MAX);
				strcpy (newPath, resolvedPath);
				this->pathList.push_back (newPath);
			}
		}
	}
	
	// イテレータを設定
	this->pathIterator = this->pathList.begin ();
}


SQChar*
SCImportPathGenerator::get (void)
{
	SQChar *result = (*(this->pathIterator));
	if( this->pathIterator == this->pathList.end () ) {
		this->saveLastEntryType = SC_ENTRY_TYPE_INVALID;
		return 0;
	}
	this->saveLastEntryType = this->getCurrentEntryType (result);
	this->pathIterator++;
    return (result);
}


ScEntryType 
SCImportPathGenerator::getEntryType (void) {
	return this->saveLastEntryType;
}

ScEntryType
SCImportPathGenerator::getCurrentEntryType (const SQChar *path)
{
	typedef std::basic_string < SQChar > SQString;
	typedef std::list < SQString > SQStringList;

	struct {
		void operator () (SQString input, const SQChar *delimiter, SQStringList &out)
		{
			std::string::size_type i;
			while( (i = input.find_first_of (delimiter)) != std::string::npos ) {
				if( i > 0 ) {
					out.push_back (input.substr (0, i));
				}
				input = input.substr (i + 1);
			}
			if( input.length () > 0) {
				out.push_back (input);
			}
		}
	}
	split;


	SQStringList splitted;
	split (path, _SC( "." ), splitted);

	SQStringList::iterator extension = splitted.end ();
	if( 0 == (*extension).compare (_SC( "nut" )) ) {
		return SC_ENTRY_TYPE_NUT;
	} 
	else
	// TODO: 環境によって拡張子が変わる可能性がある
	if( 0 == (*extension).compare (_SC( "so" )) ) {
		return SC_ENTRY_TYPE_EXTEND;
	}
	return SC_ENTRY_TYPE_INVALID;
}

void 
SCImportPathGenerator::clear (void)
{
	for( PathList::iterator 
	i  = this->pathList.begin ();
	i != this->pathList.end   (); i++
	){
		// 確保した文字列を解放する
		if( (*i) != 0 ) {
			free (*i);
		}
	}
	this->pathList.clear ();
}
