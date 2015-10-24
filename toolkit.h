/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

#ifndef toolkit_h
#define toolkit_h

/* the interface for a simple termbox based toolkit */

#include <termbox.h>
#include <stdbool.h>

typedef enum BoxType{box_type,textbox_type,buttonbox_type,box_count} BoxType;
typedef void* BoxHandle;

void box_start();
void box_finish();

BoxHandle box_make(int x, int y, int w, int h, BoxType type);
void box_unmake(BoxHandle box);

/* if passed handle, moves the box associated with it to the top
 * returns the box at the top of the stack */
BoxHandle box_top(BoxHandle box);
/* if passed handle, moves the box associated with it to the bottom
 * returns the box at the bottom of the stack */
BoxHandle box_bottom(BoxHandle box);
/* moves the box up in the stack */
void box_up(BoxHandle box);
/* moves the box down in the stack */
void box_down(BoxHandle box);


#endif
