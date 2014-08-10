#include <stdio.h>
#include <squirrel.h>
#include "sqrat.h"

/*---------------------------------------------------------------------------*/

class SqratTest {
public:
	int func (int value) {
		scprintf (_SC("sqrattest, value = %d\n"), value);
	}
};

/*---------------------------------------------------------------------------*/

extern "C" SQRESULT initialize (HSQUIRRELVM vm)
{
	using namespace Sqrat;
	Class <SqratTest> sqratTestClass (vm);
	sqratTestClass.Func (_SC("func"), &SqratTest::func);
	RootTable (vm).Bind (_SC("SqratTest"), sqratTestClass);
	return SQ_OK;
}

/*---------------------------------------------------------------------------*/

extern "C" SQRESULT finalize (HSQUIRRELVM vm)
{
	scprintf ("Finalize, Extend Library!\n");
	return SQ_OK;
}
