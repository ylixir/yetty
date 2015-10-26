/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* the interface for the string type used in the box toolkit */

#ifndef bxstr_h
#define bxstr_h

#include <stdlib.h>

typedef struct BoxStringData* bxstr;

bxstr bxstr_make(char* txt);
void bxstr_unmake(bxstr bs);
bxstr bxstr_append(bxstr bs, char* add);
bxstr bxstr_dup(bxstr bs);
bxstr bxstr_slice(bxstr bs, size_t start, size_t end);
char* bxstr_raw(bxstr bs);
#endif
