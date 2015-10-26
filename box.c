/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* the implementation of a simple termbox based toolkit */

#include "box.h"
#include "boxstr.h"

#include <string.h>
#include <stdio.h>

#define BOX_EVENTS 6

typedef struct BoxData BoxData;
struct BoxData {
  op place, size;
  bxstr text;
  bool border, ro;
  BoxHandler handlers[BOX_EVENTS];
  Box next;
};
BoxData rootbox = {{0,0},{0,0},0,false,false,{0,0,0,0,0,0},0};
Box zstack = 0;
Box default_box = 0;
Box focus_box = 0;

void box_redraw(Box b)
{
  if(b->next)
    box_redraw(b->next);

  struct tb_cell* canvas = tb_cell_buffer();
  char* i = b->text?bxstr_raw(b->text):"";
  for(size_t y=b->place.y;y<b->place.y+box_get_size(b).y;y++)
  {
    for(size_t x=b->place.x;x<b->place.x+box_get_size(b).x;x++)
    {
      if(!i) break;
      tb_utf8_char_to_unicode(&((canvas+y*box_get_size(b).x+x)->ch),i);
      i += tb_utf8_char_length(*i);
    }
  }
}

BoxHandler box_event_handler(Box b, BoxEventType type, BoxHandler handler)
{
  BoxHandler old = b->handlers[type];
  b->handlers[type]=handler;
  return old;
}
bool box_call_handler(Box b, BoxEvent bev);

bool box_default_key_handler(Box b, BoxEvent bev)
{
  switch(bev.key)
  {
  case TB_KEY_TAB:;
    Box last, next;
    last = b;
    next = last->next?last->next:zstack;
    focus_box = next;

    bev.type=BOX_EVENT_FOCUS;
    bev.key=0;
    box_call_handler(last,bev);

    bev.place = next->place;
    bev.size = next->size;
    box_call_handler(next,bev);
    break;
  case TB_KEY_ENTER:
    bev.type=BOX_EVENT_ACTIVATE;
    bev.place=default_box->place;
    bev.size=default_box->size;
    box_call_handler(default_box,bev);
    break;
  default:
    if(bev.ch)
    {
      char utf[8];
      tb_utf8_unicode_to_char(utf,bev.ch);
      if(b->text)
        bxstr_append(b->text,utf);
      else
        b->text=bxstr_make(utf);
      //tb_clear();
      box_redraw(&rootbox);
      box_redraw(zstack);
      tb_present();
    }
  }
  return true;
}
bool box_default_activate_handler(Box b,BoxEvent bev)
{
  /* kill warnings */
  Box a=bev.type?b:b+1; a=a?a:a+1;

  box_finish();
  return true;
}
bool box_default_focus_handler(Box b,BoxEvent bev)
{
  /* kill warnings */
  Box a=bev.type?b:b+1; a=a?a:a+1;

  return true;
}
BoxHandler box_default_handlers[BOX_EVENTS] =
{
  &box_default_key_handler,
  0,
  &box_default_activate_handler,
  &box_default_focus_handler,
  0,0
};

bool box_call_handler(Box b, BoxEvent bev)
{
  BoxHandler h = b->handlers[bev.type],
             d = box_default_handlers[bev.type];

  /* this mess means call the default if no handler is defined
   * or call the handler if it is defined. in any case, call the
   * default if the first thing returns false */
  return (h?h:d)(b,bev) || d(b,bev);
}

bool box_running = false;
/* starts the event loop, make sure to register callbacks first */
void box_start()
{
  box_running = tb_init() < 0 ? false:true;
  tb_select_input_mode(TB_INPUT_ESC | TB_INPUT_MOUSE);
  if(box_running)
  {
    tb_clear();
    box_redraw(&rootbox);
    box_redraw(zstack);
    tb_present();
  }

  while(box_running)
  {
    struct tb_event tev;
    BoxEvent bev;
    memset(&bev,0,sizeof(BoxEvent));

    if(!tb_poll_event(&tev)) box_running = false;
    else
    {
      switch(tev.type)
      {
      case TB_EVENT_KEY:
        bev = (BoxEvent){ BOX_EVENT_KEY,
          tev.key,tev.ch,
          focus_box->place,focus_box->size };
        box_call_handler(focus_box,bev);
        break;
      }
    }
  }
  tb_shutdown();
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
    if(b->text) bxstr_unmake(b->text);
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
  if(text)
  {
    if(b->text) bxstr_unmake(b->text);
    b->text = bxstr_make(text);
  }
  return b->text?bxstr_raw(b->text):0;
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

