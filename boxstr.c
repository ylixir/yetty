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
  size_t size;   /*number of bytes in string */
  size_t space;  /*number of bytes available */
  size_t length; /*number of characters in string */
};

bxstr bxstr_make(char* txt)
{
  bxstr bs = malloc(sizeof(struct BoxStringData));
  txt = txt?txt:"";
  if(bs)
  {
    bs->size=strlen(txt);
    bs->space=(bs->size/GROW_FACTOR+1)*GROW_FACTOR;
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
  size_t diff = bs->space-bs->size;
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
  strcpy(bs->text+bs->size,add);
  bs->size += al;
  return bs;
}

bxstr bxstr_dup(bxstr bs)
{
  bxstr newbs = malloc(sizeof(struct BoxStringData));
  if(newbs)
  {
    newbs->size = bs->size;
    newbs->space =  bs->space;
    newbs->text = malloc(bs->space);
    if(newbs->text)
      strncpy(newbs->text,bs->text,bs->size);
    else
    {
      free(newbs);
      newbs = 0;
    }
  }
  return newbs;
}

bxstr bxstr_fast_slice(bxstr bs, size_t start, size_t end)
{
  start = start > bs->size?bs->size:start;
  end = end > bs->size?bs->size:end;

  bxstr newbs = malloc(sizeof(struct BoxStringData));
  if(newbs && (newbs->text = malloc(newbs->space)))
  {
    newbs->size = end > start?end-start:start-end;
    newbs->space =  (newbs->size/GROW_FACTOR+1)*GROW_FACTOR;
    strncpy(newbs->text,bs->text+(end>start?start:end),newbs->size);
    memset(newbs->text+newbs->size,0,newbs->space-newbs->size);
  }
  else
  {
    free(newbs);
    newbs=0;
  }
  return newbs;
}

char* bxstr_raw(bxstr bs)
{
  return bs->text;
}
