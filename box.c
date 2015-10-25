/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* the implementation of a simple termbox based toolkit */

#include "box.h"

#include <stdlib.h>
#include <string.h>

typedef struct BoxData BoxData;
struct BoxData {
  op place, size;
  char* text;
  bool border, ro;
  Box next;
};
BoxData rootbox = {{0,0},{0,0},0,false,false,0};
Box zstack = 0;
Box default_box = 0;
Box focus_box = 0;

bool box_running = false;
/* starts the event loop, make sure to register callbacks first */
void box_start()
{
  box_running = true;
}
/* call to exit the event loop */
void box_finish()
{
  box_running = false;
}
/* basically the whole window, you can't unmake, or move this */
Box box_root()
{
  rootbox.size=(op){tb_width(),tb_height()};
  return &rootbox;
}

/* creates box with no text */
Box box_make(op place, op size)
{
  Box b = malloc(sizeof(BoxData));
  if(b)
  {
    memset(b,0,sizeof(BoxData));
    b->place = place;
    b->size = size;
    b->next = zstack;
    zstack = b;
    default_box=default_box?default_box:b;
    focus_box=focus_box?focus_box:b;
  };

  return b;
}
/* removes the box from memory */
void box_unmake(Box b)
{
  BoxData** i;
  for(i=&zstack; *i != b && !*i; i=&(*i)->next);
  if(*i)
  {
    *i=b->next;
    free(b);
  }
}

void box_set_place(Box b, op place)
{
  if(b)
    b->place=place;
}
op box_get_place(Box b)
{
  return b?b->place:(op){0,0};
}
void box_set_size(Box b, op size)
{
  if(b)
    b->size=size;
}
op box_get_size(Box b)
{
  return b?b->size:(op){0,0};
}
void box_set_border(Box b, bool draw) /* default is false */
{
  if(b)
    b->border=draw;
}
bool box_get_border(Box b)
{
  return b?b->border:false;
}
void box_set_read_only(Box b, bool ro) /* default is false */
{
  if(b)
    b->ro=ro;
}
bool box_get_read_only(Box b)
{
  return b?b->border:false;
}


/* pass zero/NULL if you don't wish to modify, pass "" to clear */
char* box_text(Box b, char* text) /* utf8 */
{
  if(!b)
    return 0;
  if(text)
    b->text=!text[0]?0:text;
  return b->text;
}

/* default box is activated when enter is pressed */
Box box_default(Box b) /* pass 0 to just get default */
{
  return default_box=b?b:default_box;
}
/* focus box receives key events */
Box box_focus(Box b) /* pass 0 to just get focus */
{
  return focus_box=b?b:focus_box;
}

/* modify z order, passing zero/NULL leaves the order unmodified */
Box box_top(Box box) /* returns top box */
{
  return box;
}
Box box_bottom(Box box) /* returns bottom box */
{
  return box;
}
void box_up(Box box)
{
  int i;
  i=box?1:2;
  i=i?1:2;
}
void box_down(Box box)
{
  int i;
  i=box?1:2;
  i=i?1:2;
}

