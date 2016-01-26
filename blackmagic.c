/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/* all the black magic stuff like grabbing a tty and
 * managing the password database and stuff
 * */
#include "blackmagic.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/* these two functions provide some error logging */
void fail(const char message[])
{
  perror(message);
}
void fail_and_bail(const char message[])
{
  fail(message);
  exit(-1);
}

/* this function attaches our app to the specified tty */
bool open_tty(const char tty[])
{
  /* setsid disconnects us from any controlling terminals */
  if(-1 == setsid() && getsid(0) != getpid())
    fail_and_bail("Couldn't make ourselves a new session");

  if(-1 == close(0))
    fail_and_bail("Couldn't close stdin"); /* <-- that's zero */

  /* open will return the  lowest free file descriptor.
   * we just closed 0 (stdin) */
  if(0 != open(tty,O_RDWR))
    fail_and_bail("Didn't assign the tty to stdin");
  dup2(0,1); /* stdout */
  dup2(0,2); /* stderr */

  /* i'm leaving this here, in case it's needed for portability,
   * but consider the reverse implication of O_NOCTTY
  const int steal_the_tty=1;
  if(-1 == ioctl(0,TIOCSCTTY, steal_the_tty))
    fail_and_bail("Couldn't steal the tty");

  fflush(NULL);
  */
  return true;
}

