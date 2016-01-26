/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* header/docs for all the black magic stuff like grabbing a tty and
 * managing the password database and stuff
 * */

#ifndef blackmagic_h
#define blackmagic_h

#include <stdbool.h>

/* this function attaches our app to the specified tty */
bool open_tty(const char tty[]);

#endif
