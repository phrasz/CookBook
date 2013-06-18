#ifndef _LONGLONG_H_
#define _LONGLONG_H_

// until I know how other compilers deal with this, I will assume that
// anything not msvc knows about <stdint.h> and is C99 compatible. If not,
// add a define here with the local 64-bit integer type.

// allegro defines some nice constants depending on OS, so I'm using these
// for recognition.

#include <allegro.h>

#ifdef ALLEGRO_MSVC

// define the stdint type as __int64, micro$ofts 64 bit integer
// if it doesn't like "unsigned __int64", find the appropriate type like
// "__uint64" or something.
typedef unsigned __int64 uint64_t

#else

// all other os's are assumed to have stdint.h
#include <stdint.h>

#endif

#endif
