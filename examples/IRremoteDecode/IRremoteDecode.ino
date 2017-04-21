// This sketch uses the IRrevc class of IRremote (can be installed from Arduino
// library manager). It reads IT signals with that class, and sends it 
// to DecodeIR.

// Requires around 109236 bytes of program storage space, and 2538 of RAM.
// Differently put, will not run on anything considerably smaller than
// Mega 2560.

#include <IRremote.h>
#include <decodePrintIr.h>

static const unsigned int RECV_PIN = 11;

static IRrecv irrecv(RECV_PIN);

static decode_results results;

void decode(decode_results* results) {
    //Serial.println(results->value, HEX);
    unsigned int length = results->rawlen;
    microseconds_t buf[length];
    for (unsigned int i = 0; i < length - 1; i++) {
        buf[i] = USECPERTICK*results->rawbuf[i+1];
        //Serial.print(buf[i]);
        //Serial.print(" ");
    }
    //Serial.println();
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
