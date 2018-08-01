// This sketch uses the IRrevc class of IRremote (can be installed from Arduino
// library manager). It reads IR signals with that class, and sends it
// to DecodeIR.

// Of course, the library IRremote needs be installed to compile
// and run this sketch.

// Due to the shortcomings of the IRremote library, the decodes that
// come out are not always really sensible.

// This sketch uses the IRremote library because it is widely spread and known.
// Please try my library Infrared (Infrared4Arduino) too (or instead).
// It has some advantages...

// Requires around 109236 bytes of program storage space, and 2538 of RAM.
// Differently put, will not run on anything considerably smaller than
// Mega 2560.

#include <IRremote.h>
#include <decodePrintIr.h>

static const unsigned int RECV_PIN = 47;//11;

static IRrecv irrecv(RECV_PIN);

static decode_results results;

void decode(decode_results* results) {
    //Serial.println(results->value, HEX);
    unsigned int length = results->rawlen;
    microseconds_t buf[length];
    for (unsigned int i = 0; i < length - 1; i++) {
        buf[i] = USECPERTICK*results->rawbuf[i+1];
    }
    buf[length-1] = 65000U;

    decodePrintIr(Serial, buf, length/2);
}

void setup() {
    Serial.begin(115200);
    irrecv.enableIRIn(); // Start the receiver
}

void loop() {
    if (irrecv.decode(&results)) {
        decode(&results);
        irrecv.resume(); // Receive the next value
    }
}
