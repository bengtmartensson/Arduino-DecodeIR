#ifndef _DECODEPRINTIR_H
#define _DECODEPRINTIR_H

#include <DecodeIR.h>
#include <Stream.h>

static const frequency_t DEFAULT_FREQUENCY = 38000U;

void decodePrintIr(Stream& stream, frequency_t frequency, microseconds_t* data, uint16_t intro_length, uint16_t rep_length = 0);

void decodePrintIr(Stream& stream, microseconds_t* data, uint16_t length);

void decodePrintIr(microseconds_t* data, uint16_t length);

#endif // _DECODEPRINTIR_H
