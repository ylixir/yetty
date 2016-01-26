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
 * passing notty will cause yetty to skip grabbing a tty
 * for running in xterm or something
 */

/*
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
*/

#include "box.h"
#include "blackmagic.h"

#include <string.h>

int main(int argc, char *argv[])
{
  if(1==argc)
    open_tty("/dev/tty1");
  else if(strcmp(argv[1],"notty"))
    open_tty(argv[1]);

  Box b =box_make((op){10,10},(op){10,1});
  box_text(b,"Hello");
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
