# Arduino-DecodeIR

This project contains a port of the well-known C++ library DecodeIR to the
Arduino platform. It requires somewhat more that 100kB flash space, and 2kB of RAM space.
It will thus not run on one of the smaller boards, like the Uno.

## History
The original library was written by Jon Fine. It has been extended by may others,
in particular Graham Dixon ("mathdon") and Dave Reed ("3FG").
It is "maintained" in the [JP1 forum](http://www.hifi-remote.com/forums/index.php).


It can be considered as a fork of the
[2.45 tag](https://sourceforge.net/p/controlremote/code/HEAD/tree/tags/decodeir-2.45/)
from the [official sources at SourceForge](https://sourceforge.net/p/controlremote/code/HEAD/tree/trunk/decodeir/).
My version presenty contains only the files relavant for the Arduino port.
The other files (Java JNI interface, autoconfig files, Maven pom) are found in
the original repository.

## Changes
This project is a straight-forward port of DecodeIR 2.45. It is intended to
be functionally equivalent. The main changes necessary was to use the C99
portable types (like `uint32_t` etc.) instead of the, per definition, non-portable
types like `int` and `char`. Somewhat simplified, most `int`s have been replaced
by `int32_t`, `unsigned int`s by `uint32_t`, and `char`s (used as numbers) by
`uint8_t`.
Also the dependence of std::set has been removed.
It was the only dependence of the standard C++ library.)

## API
The main file DecodeIR[h,cpp] has a single public entry point, `DecodeIR(...)`,
documented in DecodeIR.cpp (line 5412ff in the current version). A slightly more
user friendly method is presented by the wrapper file `decodePrintIr.[h,cpp].
Note that the two "lengths" are in "number of bursts", not in number of durations
(actually, the number of bursts is half the number of durations).
The example files is probably the easiest way to learn how to use the library,
using the wrapper.

## main routine
The file `main.cpp` is a small main routine, that can be used on Unix-like system
to create a command line program that will decode signal in the Pronto Hex format.
This can be useful e.g. for testing. It is not intended to run on the Arduino.
To compile, use the Makefile.

## Questions, maintainence, contributions
This project is intended as a port of the current version of DecodeIR. 
Issues with the port should be directed to me, e.g. as
[issue](https://github.com/bengtmartensson/Arduino-DecodeIR/issues).
Questions, extensions regarding protocols etc should be directed to the
[JP1 forum](http://www.hifi-remote.com/forums/index.php). 