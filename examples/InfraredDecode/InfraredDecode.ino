// This sketch uses the IrReceiverSampler class of Infrared4Arduino
// (can be installed from Arduino library manager).
// It reads IR signals with that class, and sends it
// to DecodeIR.

// It requires a demodulating sensor connected to pin RECEIVE_PIN.

// For a more capable and practically more interesting program,
// see InfraredDecoderLCD.

// Sketch uses 111722 bytes of program storage space.
// Global variables use 2491 bytes (30%) of dynamic memory.
// Will not run on anything considerably smaller than a Mega2560.

#include <IrReceiverSampler.h>
#include <decodePrintIr.h>

// Change to the pin where you have your receiver
#define RECEIVE_PIN 5U

// Get long signals in one chunk
#define ENDTIMEOUT 100U

// Max size of signals
#define BUFFERSIZE 200U

// Change if necessary
#define BAUD 115200

IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
    receiver->setEndingTimeout(ENDTIMEOUT);
}

void loop() {
    receiver->receive(); // combines enable, loop, disable

    if (receiver->isEmpty())
        Serial.println(F("timeout"));
    else {
        IrSequence *irSequence = receiver->toIrSequence();
        decodePrintIr(Serial, irSequence->getDurations(), irSequence->getLength()/2);
        delete irSequence;
    }
}
