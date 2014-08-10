#ifndef ___INCLUDED_APP_H___
#define ___INCLUDED_APP_H___


#include <squirrel.h>


/** Application class. */
class Application
{
	// constructor, destructor
	public:
		 Application ();
		~Application ();

	public:
		void init (void);
		void term (void);
		void exec (int argc, char **argv);

	private:
		void execScript (char **argv, int argc);
    void interactive ();

	private:
		HSQUIRRELVM vm;
};


#endif // ___INCLUDED_APP_H___
