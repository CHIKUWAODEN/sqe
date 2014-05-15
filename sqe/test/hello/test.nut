// SEL（.so 拡張子を持つファイル）として作成された hello モジュールのロード.
// .nut と同様に、拡張子（.so）は付けない.
// 接頭辞 lib も省略する。これはすべて補完され、libhello.so というファイルが探索される.
require ("hello");


// SEL 関数の呼び出してみる
hello();
