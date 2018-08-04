# Arduino-DecodeIR

This project contains a port of the well-known C++ library DecodeIR to the
Arduino platform. It requires somewhat more that 100kB flash space, and 2kB of RAM space.
It will not run on the smaller boards, like the Uno.

## History
The original library was written by Jon Fine, who put it into the public domain.
It has been extended by may others,
in particular Graham Dixon ("mathdon") and Dave Reed ("3FG").
It is "maintained" in the [JP1 forum](http://www.hifi-remote.com/forums/index.php).


This project can be considered as a fork of the
[2.45 tag](https://sourceforge.net/p/controlremote/code/HEAD/tree/tags/decodeir-2.45/)
from the [official sources at SourceForge](https://sourceforge.net/p/controlremote/code/HEAD/tree/trunk/decodeir/).
My version presently contains only the files relevant for the Arduino port.
The other files (Java JNI interface, autoconfig files, Maven pom) are found in
the original repository.

The official documentation is contained here as the file DecodeIR.html.

## Changes
This project is a straight-forward port of DecodeIR 2.45. It is intended to
be functionally equivalent. The main changes necessary was to use the C99
portable types (like `uint32_t` etc.) instead of the, per definition, non-portable
types like `int` and `char`. Somewhat simplified, most `int`s have been replaced
by `int32_t`, `unsigned int`s by `uint32_t`, and `char`s (used as numbers) by
`uint8_t`.

Also the dependence of `std::set` has been removed.
(This was the only dependence of the standard C++ library.)

## Dependencies
The library proper does not depend on any other Arduino library.
However, the examples do depend on [Infrared4Arduino](https://github.com/bengtmartensson/Infrared4Arduino),
[IRremote](https://github.com/z3t0/Arduino-IRremote), and/or [LiquidCrystal_I2C](https://github.com/marcoschwartz/LiquidCrystal_I2C).

Version 2.45.1 and 2.45.2 inadvertently depended on Infrared4Arduino for compilation.

## API
The main file `DecodeIR[h,cpp]` has a single public entry point, `DecodeIR(...)`,
documented in `DecodeIR.cpp` (line 5412ff in the current version). A slightly more
user friendly method is presented by the wrapper file `decodePrintIr.[h,cpp].
Note that the two "lengths" are in "number of bursts", not in number of durations.
(The number of bursts is half the number of durations).
The example files is probably the easiest way to learn how to use the library,
using the wrapper.

## main()-routine
The file `main.cpp` is a small main routine, that can be used on Unix-like system
to create a command line program that will decode signal in the Pronto Hex format.
This can be useful e.g. for testing. It is not intended to run on the Arduino.
To compile, use the supplied `Makefile`.

## Questions, maintenence, contributions
This project is intended as a port of the current version of DecodeIR.
Issues with the port should be directed to me, e.g. as
[issue](https://github.com/bengtmartensson/Arduino-DecodeIR/issues).
Questions, extensions regarding protocols etc should be directed to the
[JP1 forum](http://www.hifi-remote.com/forums/index.php).

[Here](http://www.hifi-remote.com/forums/viewtopic.php?t=100598) is a thread
in the JP1-Forum discussing the current project.

The official DecodeIR is not being developed any more. I intend this project
to be a port of the official one, and for that reason, improvements and
extensions are not really solicited.
PR in this direction will likely be rejected.

But, of course, feel free to develop your own fork; this is Open Source!