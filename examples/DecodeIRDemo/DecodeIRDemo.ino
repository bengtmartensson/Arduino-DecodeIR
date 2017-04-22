// This sketch is a static test that invokes DecodeIR on the included data.
// Expect the following output:
// protocol=NEC1 device=12 subdevice=34 obc=56 hex0=227

#include <decodePrintIr.h>

void setup() {
    Serial.begin(115200);
    Serial.println("testing decodeIR");
    frequency_t frequency = 38400U;
    uint16_t intro_length = 34U;
    uint16_t rep_length = 2U;

    microseconds_t data[] = {9041, 4507, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 1694, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 44293,
        9041, 2267, 573, 65000/*96193*/};
    
    decodePrintIr(Serial, frequency, data, intro_length, rep_length);
    Serial.println("*** end testing decodeIR ***");
}

void loop() {
}
