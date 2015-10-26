/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* and here we have the main program file */

/* usage:
 * yetty [ /dev/tty? | notty ]
 *
 * not passing a parameter will cause yetty to launch on tty1
 * passing notty for running in xterm or something
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "box.h"

/* these two functions provide some error logging */
void fail(const char message[])
{
  //char *error = strerror(errno);
  perror(message);
  //perror(error);
}
void fail_and_bail(const char message[])
{
  fail(message);
  exit(-1);
}

/* this is where the black magic happens. other than opening the tty
 * and doing the password stuff, this is a pretty standard console app
 */
void open_tty(const char tty[])
{
  /* setsid disconnects us from any controlling terminals */
  if(-1 == setsid() && getsid(0) != getpid())
    fail_and_bail("Couldn't make ourselves a new session");

  if(-1 == close(0))
    fail_and_bail("Couldn't close stdin");

  /* open will return the  lowest free file descriptor.
   * we just closed 0...  */
  if(0 != open(tty,O_RDWR))
    fail_and_bail("Didn't assign the tty to stdin");
  dup2(0,1); /* stdout */
  dup2(0,2); /* stderr */
  fflush(NULL);

  /* i'm leaving this here, in case it's needed for portability,
   * but consider the reverse implication of O_NOCTTY

  const int steal_the_tty=1;
  if(-1 == ioctl(0,TIOCSCTTY, steal_the_tty))
    fail_and_bail("Couldn't steal the tty");

  */
}

int main(int argc, char *argv[])
{
  if(1==argc)
    open_tty("/dev/tty1");
  else if(strcmp(argv[1],"notty"))
    open_tty(argv[1]);

  Box b =box_make((op){10,10},(op){10,1});
  box_text(b,"H");
  box_start();
  box_unmake(b);
  /*
  if(tb_init())
      fail("tb_init");
  tb_clear();

  int x=0,y=0;
  int xv=1,yv=1;
  while(1)
  {
    struct tb_event event;

    if(tb_peek_event(&event,50) == TB_EVENT_KEY) break;
    if(tb_width()-14 < x || x < 0) xv *= -1;
    if(tb_height() < y || y < 0) yv *= -1;

    //for(int i=0; 0 != test[i]; i++)
    //  tb_change_cell(x+i,y,' ',TB_WHITE,TB_BLACK);
    x+=xv; y+=yv;
    print(x,y,test,TB_WHITE,TB_BLACK);
    //for(int i=0; 0 != test[i]; i++)
    //  tb_change_cell(x+i,y,test[i],TB_WHITE,TB_BLACK);
    tb_present();
  }
  tb_shutdown();
*/
}
