#include <DecodeIR.h>

void setup() {
    Serial.begin(115200);
    Serial.println("testing decodeIR");
    frequency_t frequency = 38400UL;
    uint16_t intro_length = 34UL;
    uint16_t rep_length = 2UL;

    microseconds_t data[] = {9041, 4507, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 1694, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 1694, 573, 573, 573, 573, 573, 573, 573, 1694, 573, 1694, 573, 44293,
        9041, 2267, 573, 65000/*96193*/};

    uint32_t decodeir_context[2] = {0, 0};
    char protocol[255] = "";
    int32_t device = -1;
    int32_t subdevice = -1;
    int32_t obc = -1;
    int32_t hex[4] = {-1, -1, -1, -1};
    char misc_message[255] = "";
    char error_message[255] = "";

    while (true) {
        DecodeIR(decodeir_context, data, frequency, intro_length, rep_length,
                protocol, &device, &subdevice, &obc, hex, misc_message,
                error_message);

        if (protocol[0] == '\0')
            break;

        Serial.print(F("protocol="));
        Serial.print(protocol);
        Serial.print(F(" device="));
        Serial.print(device);
        if (subdevice != -1) {
            Serial.print(F(" subdevice="));
            Serial.print(subdevice);
        }
        Serial.print(F(" obc="));
        Serial.print(obc);
        for (unsigned int i = 0; i < 4; i++) {
            if (hex[i] != -1) {
                Serial.print(F(" hex"));
                Serial.print(i);
                Serial.print("=");
                Serial.print(hex[i]);
            }
        }

        if (misc_message[0]) {
            Serial.print(F(" misc="));
            Serial.print(misc_message);
        }
        if (error_message[0]) {
            Serial.print(F(" error="));
            Serial.print(error_message);
        }
        Serial.println();

    }
    Serial.println("*** end ***");
}

void loop() {
}