// This sketch uses the IrReceiverSampler class of Infrared4Arduino (can be installed from Arduino
// library manager). It reads IR signals with that class, and sends it 
// to DecodeIR.

// It requires a demodulating sensor connected to pin RECEIVE_PIN.

// Sketch uses 111722 bytes of program storage space. 
// Global variables use 2491 bytes (30%) of dynamic memory.
// Will not run on anything considerably smaller than a Mega2560.

#include <IrReceiverSampler.h>
#include <DecodeIRClass.h>

#define RECEIVE_PIN 5U
#define BUFFERSIZE 200U
#define BAUD 115200

IrReceiver *receiver;

void setup() {
    Serial.begin(BAUD);
    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
}

void loop() {
    receiver->receive(); // combines enable, loop, disable

    if (receiver->isEmpty())
        Serial.println(F("timeout"));
    else {
        IrSequence *irSequence = receiver->toIrSequence();
        DecodeIRClass decoder(*irSequence, IrSignal::defaultFrequency);
        delete irSequence;
        if (decoder.getProtocol()[0] != '\0')
            Serial.println(decoder.getString1());
    }
}
