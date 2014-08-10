Util <-
{
	function printSystemInfo () {
		print( format (
			"Squirrel version number = %d \n" +
			"Squirrel version = %s \n" +
			"sizeof character = %d \n" +
			"sizeof integer = %d \n" +
			"sizeof float = %d \n"
			, ::_versionnumber_
			, ::_version_
			, ::_charsize_
			, ::_intsize_
			, ::_floatsize_
		));
	}
}

function setLogFlag (logtype, flag)
{
	if( (logtype == "error") ||	(logtype == "e") ) {
		Debug.e = flag;
	}
	else
	if( (logtype == "debug") ||	(logtype == "d") ) {
		Debug.d = flag;
	}
	else
	if( (logtype == "info") ||	(logtype == "i") ) {
		Debug.i = flag;
	}
}

Debug <- 
{
	function etrace (log) {
		if( Debug.e ) { print (log); }
	},
	function dtrace (log) {
		if( Debug.d ) { print (log); }
	},
	function itrace (log) {
		if( Debug.i ) { print (log); }
	},
	e = false,
	d = false,
	i = false
}
