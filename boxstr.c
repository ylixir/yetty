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
#include <stdbool.h>

const int GROW_FACTOR=100;
struct BoxStringData
{
  char* text;
  size_t size;   /*number of bytes in string */
  size_t space;  /*number of bytes available */
};

bxstr bxstr_make(char* text)
{
  bxstr bs;
  size_t size,space;

  /*if they pass in a null string then make an empty string*/
  text = text?text:"";

  /*find out how much room we need
    it's annoying that we have to scan the string twice*/
  size = strlen(text);
  space = (size/GROW_FACTOR+1)*GROW_FACTOR;

  if((bs=malloc(sizeof(struct BoxStringData)))
    && (bs->text=malloc(space)))
  {
    bs->space = space;
    bs->size = size;
    strcpy(bs->text, text);
  }
  else
  {
    free(bs);
    bs = 0;
  }
  return bs;
}

void bxstr_unmake(bxstr bs)
{
  if(bs) free(bs->text);
  free(bs);
}

bxstr bxstr_append(bxstr bs, char* add)
{
  size_t diff = bs->space - bs->size;
  size_t add_len = strlen(add);
  if(diff <= add_len)
  {
    size_t newspace = bs->space+(diff/GROW_FACTOR+1)*GROW_FACTOR;
    char* newtext = malloc(newspace);
    if(!newtext)
      return 0;
    else
    {
      strncpy(newtext,bs->text,bs->size);
      free(bs->text);
      bs->text = newtext;
      bs->space= newspace;
    }
  }
  strcpy(bs->text+bs->size,add);
  bs->size += add_len;
  return bs;
}

bxstr bxstr_concat(bxstr first, bxstr second)
{
  bxstr new_bs;
  size_t new_space = ((first->size+second->size)/GROW_FACTOR+1)*GROW_FACTOR;
  if((new_bs=malloc(sizeof(struct BoxStringData)))
   &&(new_bs->text=malloc(new_space)))
  {
    new_bs->size = first->size+second->size;
    new_bs->space = new_space;
    strncpy(new_bs->text,first->text,first->size);
    strncpy(new_bs->text+first->size,second->text,second->size+1);
  }
  else
  {
    free(new_bs);
    new_bs = 0;
  }
  return new_bs;
}

bxstr bxstr_dup(bxstr bs)
{
  bxstr new_bs;
  if((new_bs= malloc(sizeof(struct BoxStringData)))
   &&(new_bs->text = malloc(bs->space)))
  {
    new_bs->size = bs->size;
    new_bs->space =  bs->space;
    strncpy(new_bs->text,bs->text,bs->size+1);
  }
  else
  {
    free(new_bs);
    new_bs = 0;
  }
  return new_bs;
}

bxstr bxstr_slice(bxstr bs, size_t start, size_t end)
{
  start = start > bs->size
          ? bs->size          : start;
  end = end > bs->size
        ?bs->size
        :end;

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

size_t bxstr_length(bxstr bs)
{
  bool good_length = true;
  size_t length = 0;

  for(size_t i=0; i<bs->size; i++)
  {
    /*just skim over the single byte utf8 chars*/
    for(;i<bs->size && 0==(bs->text[i]&0x80);i++)
      length++;
    /*note that at this point we are one byte past
      where our outer loop thinks we are*/
    if(0xE0 == (bs->text[i]&0xC0))    {
    /* two byte sequence */
      if(i+1<bs->size
      && 0x80==(bs->text[i+1]&0xC0))
        length++;
      else
        good_length = false;
    }
    else if(0xF0 == (bs->text[i]&0xE0))
    {
    /* three byte sequence */
      if(i+2<bs->size
      && 0x80==(bs->text[i+1]&0xC0)
      && 0x80==(bs->text[i+2]&0xC0))
      {
        length++;
        i+=1;
      }
      else
        good_length = false;
    }
    else if(0xF8 == (bs->text[i]&0xF0))
    {
    /* four byte sequence */
      if(i+3<bs->size
      && 0x80==(bs->text[i+1]&0xC0)
      && 0x80==(bs->text[i+2]&0xC0)
      && 0x80==(bs->text[i+3]&0xC0))
      {
        length++;
        i+=2;
      }
      else
        good_length = false;
    }
    else if(0xFC == (bs->text[i]&0xF8))
    {
    /* five byte sequence */
      if(i+4<bs->size
      && 0x80==(bs->text[i+1]&0xC0)
      && 0x80==(bs->text[i+2]&0xC0)
      && 0x80==(bs->text[i+3]&0xC0)
      && 0x80==(bs->text[i+4]&0xC0))
      {
        length++;
        i+=3;
      }
      else
        good_length = false;
    }
    else if(0xFE == (bs->text[i]&0xFC))
    {
    /* six byte sequence */
      if(i+5<bs->size
      && 0x80==(bs->text[bs->size+1]&0xC0)
      && 0x80==(bs->text[bs->size+2]&0xC0)
      && 0x80==(bs->text[bs->size+3]&0xC0)
      && 0x80==(bs->text[bs->size+4]&0xC0)
      && 0x80==(bs->text[bs->size+5]&0xC0))
      {
        length++;
        i+=4;
      }
      else
        good_length = false;
    }
    else if(0 == bs->text[i])
    {
    /* end of the string */
      break;
    }
    else
    {
    /* invalid byte */
      good_length=false;
    }
  }
  if(false == good_length)
    return 0;
  else
    return length;
}

size_t bxstr_size(bxstr bs)
{
  return bs->size;
}

char* bxstr_raw(bxstr bs)
{
  return bs->text;
}
