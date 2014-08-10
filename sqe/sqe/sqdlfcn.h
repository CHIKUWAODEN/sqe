#ifndef ___INCLUDED_EXTEND_H___
#define ___INCLUDED_EXTEND_H___


#include <squirrel.h>


extern void* sc_dlopen (SQChar *loadpath);
extern const SQChar* sc_dlerror (void);
extern void *sc_dlsym (void *handle, const SQChar *symbol);
extern int sc_dlclose (void *handle);


#endif // ___INCLUDED_EXTEND_H___
