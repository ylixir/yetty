# yetty - a getty dm
This program is meant to be something in between a getty and a display
manager. It runs on a virtual console just like a getty, but is interactive
much like a dm. It will allow logging straight into a X session, but without
the overhead or instability of a full dm. It will also allow choosing
shells or desktop environments to log into, unlike a tty.

It is meant to be run from a virtual console, and for now, it would
be inappropriate to try to run this over a serial link or network.

It should also serve as a learning tool. If you want to see how
I figured out the black magic, take a look at the references.


## Requirements
* A C compiler
* Make
* Termbox

## License
```
Copyright © 2015 Jon "ylixir" Allen <ylixir@gmail.com>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
```
