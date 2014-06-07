/* C/C++ standard libs */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdarg.h>
#include <getopt.h>

/* Squirrel API and standaed libs */
#include <squirrel.h>
#include <sqstdaux.h>
#include <sqstdblob.h>
#include <sqstdio.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdsystem.h>

/* application */
#include "loadpath.h"
#include "libsc.h"
#include "app.h"
#include "version.h"

//-----------------------------------------------------------------------------

#ifdef SQUNICODE
#include <wchar.h>
#define sqefprintf fwprintf
#define sqevprintf vfwprintf
#define sqeprintf wprintf
#else
#define sqefprintf fprintf
#define sqevprintf vfprintf
#define sqeprintf printf
#endif

//-----------------------------------------------------------------------------

// Definition break-able block
#ifndef		BREAKABLE
#	define	BREAKABLE(a, b) for(bool a##b=true;a##b;a##b=false)
#endif
#if defined(BREAKABLE) 
#	define	breakable BREAKABLE (__LINE__,__FILE__)
#endif

//-----------------------------------------------------------------------------

void printVersion (void) {
	sqeprintf (
		_SC( "sqe version %d.%d.%d, Copyright (c) 2012 @kandayasu.\n" )
		, VERSION_NUMBER_MAJOR  
		, VERSION_NUMBER_MINOR  
		, VERSION_NUMBER_RELEASE
	);
}

//-----------------------------------------------------------------------------

void printUsage (void)
{
	printVersion ();
	sqefprintf(stderr,
		_SC("Available options are:\n")
    _SC("-i, --interactive\n")
    _SC("    run sqe as interactive shell mode.\n")
    _SC("-d, --debug\n")
    _SC("    generates debug infos.\n")
    _SC("-h, --help\n")
    _SC("   show help.\n")
    _SC("-v, --version\n")
    _SC("   show version info.\n")
  );
}

//-----------------------------------------------------------------------------

void printFunc (HSQUIRRELVM vm, const SQChar *format, ...)
{
	va_list vl;
	va_start (vl, format);
	sqevprintf (stdout, format, vl);
	va_end (vl);
}

//-----------------------------------------------------------------------------

void errorFunc (HSQUIRRELVM vm, const SQChar *format, ...)
{
	va_list vl;
	va_start (vl, format);
	sqevprintf (stderr, format, vl);
	va_end (vl);
}


//=============================================================================
// Application
//=============================================================================

Application::Application () 
: vm (0)
{}

//-----------------------------------------------------------------------------

Application::~Application () {
	this->term (); 
}

//-----------------------------------------------------------------------------

void Application::init (void)
{
	// VM initialize
	vm = sq_open (1024);
	sq_setprintfunc (
		this->vm,
		printFunc,
		errorFunc
	);
	sq_pushroottable         (this->vm);
	sqstd_register_bloblib   (this->vm);
	sqstd_register_iolib     (this->vm);
	sqstd_register_systemlib (this->vm);
	sqstd_register_mathlib   (this->vm);
	sqstd_register_stringlib (this->vm);
	sqstd_seterrorhandlers   (this->vm);
	sq_pop                   (this->vm, 1);

	sc_initialize_loadpath   ();
	sq_pushroottable         (this->vm);
	sc_register_sclib        (this->vm);
	sq_pop                   (this->vm, 1);
}

//-----------------------------------------------------------------------------

void Application::term (void)
{
	// 拡張ライブラリ類の開放と Squirrel VM の終了
	if( this->vm ) {
		sc_finalize_extend_libs (this->vm);
		sq_close (this->vm);
		this->vm = 0;
	}
}

//-----------------------------------------------------------------------------

/* TODO support interacive shell */
void Application::exec (int argc, char **argv)
{
	int result = 0;
	while( true )
	{
		// opt parse
    static const char *shortOptions = "hvdi";
		static struct option longOptions [] =
		{
			{ "help"        , no_argument, 0, 'h' },
			{ "version"     , no_argument, 0, 'v' },
			{ "debug"       , no_argument, 0, 'd' },
			{ "interactive" , no_argument, 0, 'i' },
			{ 0, 0, 0, 0 }
		};
		int optionIndex = 0;
		result = getopt_long (argc, argv, shortOptions, longOptions, &optionIndex);
		if( -1 == result ) {
			break;
		}
		switch( result ) {
			case 'h': { printUsage   (); } return;
			case 'v': { printVersion (); } return;
			case 'd': { /* this -> enableFlag (FLAG_GEN_DEBUG_INFO);      */ } 
			case 'i': { /* this -> enableFlag (FLAG_EXECUTE_INTERACTIVE); */ }
			/*
			// 値を取る引数の場合は外部変数 optarg にその値を格納する.
			case 'd':
			case 'e':
				sqefprintf (stdout, _SC("%c %s\n"), c, optarg);
		  		break;
			*/
			// 値を取る引数に値がなかった場合 ":" を返す
			case ':': {
				sqefprintf (stderr, _SC("%c needs value\n"), result);
				break;
			}
			// getoptの引数で指定されなかったオプションを受け取ると "?" を返す
			case '?': {
        sqefprintf (stderr, _SC("unknown\n"));
				break;
			}
		}
	}
	// getopt は optindに「次に処理すべき引数のインデクスを格納している.
	if( optind < argc ) {
		this->execScript (
			argv + optind,
			argc - optind
		);
	}
}

//-----------------------------------------------------------------------------

void Application::execScript (char **argv, int argc)
{
	char *filename = argv[ 0 ];
	bool loadFailed = false;
	SQInteger retval = 0;
	breakable
	{
		// load script file
		sq_pushroottable (this->vm);
		if( SQ_FAILED( sqstd_loadfile (this->vm, filename, SQTrue) ) ) {
			loadFailed = true;
			break;
		}

		// set script arguments
		sq_pushroottable (this->vm);
		for( int i = 1; i < argc; i++ ) {
			const SQChar *scratchPad = argv[ i ];
			sq_pushstring (this->vm, scratchPad , -1);
		}
		// call closure.
		if( SQ_SUCCEEDED( sq_call (this->vm, argc, SQTrue, SQTrue) ) ) {
			SQObjectType type = sq_gettype (this->vm, -1);
			if( type == OT_INTEGER ) {
				retval = type;
				sq_getinteger (this->vm, -1, &retval);
        // TODO set exit value
			}
		} else {
			printf ("call error\n");
		}
	}
	// エラーが起きた場合はこのブロックが処理される
	if( loadFailed ) {
		const SQChar *error = 0;
		sq_getlasterror (this->vm);
		if( SQ_SUCCEEDED( sq_getstring (this->vm, -1, &error) ) ) {
			scprintf (_SC( "Error [%s]\n" ), error);
		}
	}
}

//-----------------------------------------------------------------------------

void Application::interactive()
{
  // TODO
}
