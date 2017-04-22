// Copying and distribution of this file, with or without modification,
// are permitted in any medium without royalty provided the copyright
// notice and this notice are preserved.  This file is offered as-is,
// without any warranty.

// Author: Bengt Martensson

#ifndef ARDUINO
#include "DecodeIR.h"
#include <iostream>

void usage(const char* name) {
    std::cerr << "Usage:" << std::endl << "\t" << name << " <pronto hex>" << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc < 5)
        usage(argv[0]);
    
    unsigned int i = 1;
    int type;
    sscanf(argv[i++], "%x", &type);
    if (type != 0) {
        std::cerr << "Can only handle type 0000\n";
        exit(1);
    }
    int fcode;
    sscanf(argv[i++], "%x", &fcode);
    frequency_t frequency = (frequency_t) (1000000.0 / ((double) fcode * 0.241246));
    uint32_t intro_length;
    uint32_t rep_length;
    sscanf(argv[i++], "%x", &intro_length);
    sscanf(argv[i++], "%x", &rep_length);

    microseconds_t *data = new microseconds_t[2 * (intro_length + rep_length)];
    for (unsigned j = 0; j < 2 * (intro_length + rep_length); j++) {
        int ncycles;
        sscanf(argv[i++], "%x", &ncycles);
        data[j] = (int) ((1000000.0 / frequency * ncycles)/* + 0.5*/);
    }
    uint32_t decodeir_context[2] = {0, 0};
    char protocol[255] = "";
    int32_t device = (uint32_t) -1;
    int32_t subdevice = (uint32_t) -1;
    int32_t obc = (uint32_t) -1;
    int32_t hex[4] = { -1, -1, -1, -1};
    char misc_message[255] = "";
    char error_message[255] = "";

    while (true) {
        DecodeIR(decodeir_context, data, frequency, intro_length, rep_length,
                protocol, &device, &subdevice, &obc, hex, misc_message, error_message);

        if (protocol[0] == '\0')
            break;

        std::cout
                << "protocol=" << protocol
                << " device=" << device
                << " subdevice=" << subdevice
                << " obc=" << obc
                << " hex0=" << hex[0]
                << " hex1=" << hex[1]
                << " hex2=" << hex[2]
                << " hex3=" << hex[3]
                << " misc=" << misc_message
                << " error=" << error_message << "\n";
    }
}

#endif // ARDUINO
