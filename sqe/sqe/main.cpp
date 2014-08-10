#include "app.h"
#include <stdlib.h>

/** entry point */
int main (int argc, char **argv)
{
	Application *app = new Application ();
	if( app ) {
		app -> init ();
		app -> exec (argc, argv);
		app -> term ();
		delete app;
	}
	exit (EXIT_SUCCESS);
}
