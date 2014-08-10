// require による .nut ファイルの呼び出し.
// 拡張子は記述しないので注意すること.
require ("test_import_a");
require ("test_import_b");

// import によって定義された関数をコールしてみる.
// この呼び出しでは、test_import_b.nut で一番最後に定義された foo() が呼び出される
foo ();
