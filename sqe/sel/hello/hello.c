#include <stdio.h>
#include <squirrel.h>

/*---------------------------------------------------------------------------*/

static SQInteger hello (HSQUIRRELVM vm) {
	scprintf (_SC("Hello, Extend Library!\n"));
	return 0;
}

/*---------------------------------------------------------------------------*/

SQRESULT initialize (HSQUIRRELVM vm)
{
	SQRESULT result = SQ_OK;

	// hello 関数のバインド処理
	sq_pushstring (vm, _SC( "hello" ), -1);
	sq_newclosure (vm, hello, 0);
	sq_setparamscheck (vm, 1, _SC( "." ));
	sq_setnativeclosurename (vm, -1, _SC( "hello" ));
	if( SQ_FAILED( sq_newslot (vm, -3, SQFalse) ) ) {
		result = SQ_ERROR;
		sq_getlasterror (vm);
		const SQChar *error = 0;
		if( SQ_SUCCEEDED( sq_getstring (vm, -1, &error) ) ) {
			scprintf (_SC( "Error [%s]\n" ), error);
		}
	}
	return result;
}

/*---------------------------------------------------------------------------*/

SQRESULT finalize (HSQUIRRELVM vm)
{
	scprintf ("Finalize, Extend Library!\n");
	return SQ_OK;
}
