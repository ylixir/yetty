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
#include <stdlib.h>
#include <termbox.h>

typedef struct BoxData* Box; /* convenience type */
typedef struct {size_t x, y;} op; /* ordered pair */

/* data types for the event system. compare to termbox for more info */
typedef enum {
  BOX_EVENT_KEY,
  BOX_EVENT_MOUSE,
  BOX_EVENT_ACTIVATE, /* called for default box when enter pressed */
  BOX_EVENT_FOCUS,  /* called for boxes that lose/receive focus */
  BOX_EVENT_MOVE,
  BOX_EVENT_RESIZE
} BoxEventType;
typedef struct {
  BoxEventType type;
  uint16_t key;
  uint32_t ch;
  op position, size;
} BoxEvent;
typedef bool (*BoxHandler)(Box,BoxEvent);
extern const BoxHandler clear_handler, get_handler;
/* if you wish to clear an event just pass clear_handler
 * if you wish to get an event just pass get_handler
 * the function returns whatever the handler was before
 * it was modified or -1 for error */
BoxHandler box_event_handler(Box b, BoxEventType type, BoxHandler handler);

/* starts the event loop, make sure to register callbacks first */
void box_start();
/* call to exit the event loop */
void box_finish();
/* basically the whole screen, you can't unmake, move or resize this */
Box box_root();

/* creates box at with no text */
Box box_make(op position, op size);
/* removes the box from memory */
void box_unmake(Box b);

void box_set_position(Box b, op position);
op box_get_position(Box b);
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

#endif
