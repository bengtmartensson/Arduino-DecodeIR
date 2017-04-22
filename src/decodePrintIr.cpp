// Copying and distribution of this file, with or without modification,
// are permitted in any medium without royalty provided the copyright
// notice and this notice are preserved.  This file is offered as-is,
// without any warranty.

// Author: Bengt Martensson

#include <decodePrintIr.h>
#include <DecodeIR.h>

void decodePrintIr(Stream& stream, frequency_t frequency, const microseconds_t* data, uint16_t intro_length, uint16_t rep_length) {
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

        stream.print(F("protocol="));
        stream.print(protocol);
        stream.print(F(" device="));
        stream.print(device);
        if (subdevice != -1) {
            stream.print(F(" subdevice="));
            stream.print(subdevice);
        }
        stream.print(F(" obc="));
        stream.print(obc);
        for (unsigned int i = 0; i < 4; i++) {
            if (hex[i] != -1) {
                stream.print(F(" hex"));
                stream.print(i);
                stream.print("=");
                stream.print(hex[i]);
            }
        }

        if (misc_message[0]) {
            stream.print(F(" misc="));
            stream.print(misc_message);
        }
        if (error_message[0]) {
            stream.print(F(" error="));
            stream.print(error_message);
        }
        stream.println();

    }
}

void decodePrintIr(Stream& stream, const microseconds_t* data, uint16_t length) {
    decodePrintIr(stream, DEFAULT_FREQUENCY, data, length, 0);
}
