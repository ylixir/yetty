/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* the interface for the string type used in the box toolkit */

#include "boxstr.h"

#include <string.h>

const int GROW_FACTOR=100;
struct BoxStringData
{
  char* text;
  size_t length;
  size_t space;
};

bxstr bxstr_make(char* txt)
{
  bxstr bs = malloc(sizeof(struct BoxStringData));
  txt = txt?txt:"";
  if(bs)
  {
    bs->length=strlen(txt);
    bs->space=(bs->length/GROW_FACTOR+1)*GROW_FACTOR;
    bs->text=malloc(bs->space);
    if(bs->text)
      strncpy(bs->text,txt,bs->space);
    else
    {
      free(bs);
      bs = 0;
    }
  }
  return bs;
}

void bxstr_unmake(bxstr bs)
{
  if(bs->text) free(bs->text);
  free(bs);
}

bxstr bxstr_append(bxstr bs, char* add)
{
  size_t diff = bs->space-bs->length;
  size_t al = strlen(add);
  if(diff <= al)
  {
    size_t newspace = bs->space+(diff/GROW_FACTOR+1)*GROW_FACTOR;
    char* newtext = malloc(newspace);
    if(!newtext)
      return 0;
    else
    {
      strncpy(newtext,bs->text,newspace);
      free(bs->text);
      bs->text = newtext;
      bs->space= newspace;
    }
  }
  strcpy(bs->text+bs->length,add);
  bs->length += al;
  return bs;
}

bxstr bxstr_dup(bxstr bs)
{
  bxstr newbs = malloc(sizeof(struct BoxStringData));
  if(newbs)
  {
    newbs->length = bs->length;
    newbs->space =  bs->space;
    newbs->text = malloc(bs->space);
    if(newbs->text)
      strncpy(newbs->text,bs->text,bs->length);
    else
    {
      free(newbs);
      newbs = 0;
    }
  }
  return newbs;
}

bxstr bxstr_slice(bxstr bs, size_t start, size_t end)
{
  start = start > bs->length?bs->length:start;
  end = end > bs->length?bs->length:end;

  bxstr newbs = malloc(sizeof(struct BoxStringData));
  if(newbs)
  {
    newbs->length = end > start?end-start:start-end;
    newbs->space =  (newbs->length/GROW_FACTOR+1)*GROW_FACTOR;
    newbs->text = malloc(newbs->space);
    if(newbs->text)
    {
      strncpy(newbs->text,bs->text+(end>start?start:end),newbs->length);
      memset(newbs->text+newbs->length,0,newbs->space-newbs->length);
    }
    else
    {
      free(newbs);
      newbs = 0;
    }
  }
  return newbs;
}

char* bxstr_raw(bxstr bs)
{
  return bs->text;
}
