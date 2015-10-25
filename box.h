/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* the interface for a simple termbox based toolkit */

#ifndef box_h
#define box_h

#include <stdbool.h>
#include <termbox.h>

typedef struct BoxData* Box; /* convenience type */
typedef struct {int x, y;} op; /* ordered pair */

/* starts the event loop, make sure to register callbacks first */
void box_start();
/* call to exit the event loop */
void box_finish();
/* basically the whole window, you can't unmake, or move this */
Box box_root();

/* creates box at with no text */
Box box_make(op place, op size);
/* removes the box from memory */
void box_unmake(Box b);

void box_set_place(Box b, op place);
op box_get_place(Box b);
void box_set_size(Box b, op size);
op box_get_size(Box b);
void box_set_border(Box b, bool draw); /* default is false */
bool box_get_border(Box b);
void box_set_read_only(Box b, bool ro); /* default is false */
bool box_get_read_only(Box b);


/* pass zero/NULL if you don't wish to modify, pass "" to clear */
char* box_text(Box b, char* text); /* utf8 */

/* default box is activated when enter is pressed */
Box box_default(Box b); /* pass 0 to just get default */
/* focus box receives key events */
Box box_focus(Box b); /* pass 0 to just get focus */

/* modify z order, passing zero/NULL leaves the order unmodified */
Box box_top(Box box); /* returns top box */
Box box_bottom(Box box); /* returns bottom box */
void box_up(Box box);
void box_down(Box box);

/* event callback types - if you handle the callback, return true */
typedef bool (*BoxKey)(Box b, uint8_t mod, uint32_t chr);
typedef bool (*BoxMouse)(Box b, op place, uint16_t button);
typedef bool (*BoxResize)(Box b, op size);
typedef bool (*BoxActivate)(Box b);
typedef bool (*BoxFocus)(Box b);

/* pass 0 for a handler to unset any handlers */
void box_key_handler(Box b, BoxKey handler);
void box_mouse_handler(Box b, BoxMouse handler);
void box_resize_handler(Box b, BoxResize handler);
void box_activate_handler(Box b, BoxActivate handler);
/* focus is called for focus received and lost
 * use box_focus to check which */
void box_focus_handler(Box b, BoxFocus handler);

#endif
