// Copying and distribution of this file, with or without modification,
// are permitted in any medium without royalty provided the copyright
// notice and this notice are preserved.  This file is offered as-is,
// without any warranty.

#ifndef _DECODEPRINTIR_H
#define _DECODEPRINTIR_H

#include <DecodeIR.h>
#include <Stream.h>

static const frequency_t DEFAULT_FREQUENCY = 38000U;

void decodePrintIr(Stream& stream, frequency_t frequency, const microseconds_t* data, uint16_t intro_length, uint16_t rep_length = 0);

void decodePrintIr(Stream& stream, const microseconds_t* data, uint16_t length);

#endif // _DECODEPRINTIR_H
