/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* the implementation of the toolkit */

#include "toolkit.h"

#include <stdlib.h>
#include <string.h>

typedef struct Box Box;
typedef struct TextBox TextBox;
typedef struct ButtonBox ButtonBox;

struct Box
{
  int x,y;
  int w,h;
};
struct TextBox
{
  int x,y;
  int w,h;
  uint32_t *text;
};
struct ButtonBox
{
  int x,y;
  int w,h;
  uint32_t *text;
  void (*activate)(void);
  bool isdefault;
};
size_t BoxSize[box_count]={sizeof(Box),sizeof(TextBox),sizeof(ButtonBox)};

typedef struct zelement zelement;
struct zelement
{
  BoxHandle this;
  BoxType type;
  zelement* next;
};

bool print(int x, int y, uint32_t *str, uint16_t fg, uint16_t bg);

zelement *zstack=0;
zelement *focus=0;
bool box_running=true;

void box_start()
{
  if(tb_init())
    return;

  tb_clear();
  
  while(box_running)
  {
    struct tb_event event;
    uint32_t hello[6]={'h','e','l','l','o',0};
    switch(tb_poll_event(&event))
    {
    case TB_EVENT_KEY:
      print(0,0,hello,TB_BLACK,TB_WHITE);
      tb_present();
      box_finish();
      break;
    default:
      box_finish();
    }
  }
}
void box_finish()
{
  box_running=false;
}

BoxHandle box_make(int x, int y, int w, int h, BoxType type)
{
  BoxHandle box;
  zelement* new_top;
  if((box = malloc(BoxSize[type])) && (new_top = malloc(sizeof(zelement))))
  {
    memset(box,0,BoxSize[type]);
    *(Box*)box=(Box){.x=x,.y=y,.w=w,.h=h};
    new_top->this=box;
    new_top->type=textbox_type;
    new_top->next=zstack;
    zstack=new_top;
    focus=new_top;
  }
  else if(box)
  {
    free(box);
    box = 0;
  }

  return box;
}
void box_unmake(BoxHandle box)
{
  zelement* e;
  zelement** i = &zstack;
  for(; (*i)->this != box; i=&((*i)->next));
  e=*i; *i=e->next;
  free(e); free(box);
  if(focus==e)
    focus=zstack;
}

void box_up(BoxHandle box)
{
  zelement* e;
  zelement** i = &zstack;

  if(zstack->this ==  box) return;

  for(; (*i)->next->this != box; i=&((*i)->next));
  e=(*i)->next;
  if(e->next)
  {
    *i=e->next;
    e->next=(*i)->next;
  }
}
void box_down(BoxHandle box)
{
  box_up(box);
}

bool print(int x, int y, uint32_t *str, uint16_t fg, uint16_t bg)
{
  int w = tb_width();
  struct tb_cell *cells = tb_cell_buffer();
  cells += y*w;
  cells += x;
  for(int i=0; str[i] != 0 && x+i<w; i++)
    *(cells+i)=(struct tb_cell){.ch=str[i], .fg=fg, .bg=bg};
  return true;
}

