/*
 * Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 *
 */

/******* yetty - a getty replacement ********
 *
 * this program is meant to be something in between a getty and a dm
 * it runs on a virtual console just like a getty, but is interactive
 * much like a dm. it will allow logging straight into x, but without
 * the overhead or instability of a full dm. it will also allow choosing
 * shells or desktop environments to log into, unlike a tty.
 *
 * it is meant to be run from a virtual console, and for now, it would
 * be inappropriate to try to run this over a serial link or network.
 *
 * it should also serve as a learning tool. so if you want to see how
 * i figured out the black magic, take a look at the references.
 */


void open_tty(const char tty[])
{
}

int main(int argc, char *argv[])
{
}
