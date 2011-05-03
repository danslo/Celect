#ifndef GSOPCODES_H
#define GSOPCODES_H

// TODO: May want to implement this with an STL container.

#include "types.h"

/* Define the actual enum. */
#define ENUM_START(NAME) enum NAME {
#define ENUM_ADD(NAME) NAME,
#define ENUM_END };
#include "gsopcodes_internal.h"

/* String representations for debugging. */
#undef ENUM_START
#undef ENUM_ADD
#undef ENUM_END
#define ENUM_START(NAME) const char *##NAME##_to_string(u8 val) { switch (val) {
#define ENUM_ADD(NAME) case NAME: return #NAME;
#define ENUM_END default: return "Invalid value"; } }
#include "gsopcodes_internal.h"

#endif