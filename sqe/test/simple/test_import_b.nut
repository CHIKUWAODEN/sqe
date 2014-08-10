print ("load test_import_b.nut\n");

function foo () {
	print ("test_import_b.nut, foo() (1)\n");
}

// Squirrel では関数が多重で定義された場合、
// 後から定義されたものでそれまでの定義が上書きされることになる.
function foo () {
	print ("test_import_b.nut, foo() (2)\n");
}
