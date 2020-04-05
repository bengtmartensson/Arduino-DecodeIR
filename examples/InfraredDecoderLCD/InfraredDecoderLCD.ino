// This sketch uses the IrReceiverSampler class of Infrared4Arduino
// (can be installed from Arduino library manager).
// It reads IR signals with that class, and sends it
// to DecodeIRClass, which is an OO-encapsulation of DecodeIR.

// It requires either a demodulating sensor connected to pin RECEIVE_PIN,
// or a non-demodulating sensor (e.g. TSMP58000).
// The latter is recommended.

// Optionally, the decode is "printed" on an LCD display using the
// LiquidCrystal_I2C library, version 1.1.2 or later.

// Sketch uses 111722 bytes of program storage space.
// Global variables use 2491 bytes (30%) of dynamic memory.
// Will not run on anything considerably smaller than a Mega2560.

// Define for using a non-demodulating sensor.
// Otherwise, a traditional // modulating sensor, like a TSMP* will be used.
#define NON_DEMODULATING_SENSOR

// Define to have output printed to the Serial.
#define USE_SERIAL

// Define to use an LCD display connected over I2C using the
// LiquidCrystal_I2C library version 1.1.2 or later.
#define LCD

#define LCD_I2C_ADDRESS 0x3F
#define LCD_COLUMNS 20
#define LCD_ROWS 4

#ifdef NON_DEMODULATING_SENSOR
#define RECEIVE_PIN 47U
#else
#define RECEIVE_PIN 5U
#endif
#define BUFFERSIZE 400U
#define BAUD 115200
#define ENDTIMEOUT 100U
#define BEGINTIMEOUT 65535U

#include "DecodeIRClass.h"

#ifdef NON_DEMODULATING_SENSOR
#include <IrWidgetAggregating.h>
#else
#include <IrReceiverSampler.h>
#endif

#ifdef LCD
#include <LiquidCrystal_I2C.h>
#endif

IrReader *receiver;
#ifdef LCD
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);
#endif

void setup() {
#ifdef USE_SERIAL
    Serial.begin(BAUD);
    while (!Serial)
        ;
    Serial.println(F("Welcome to DecodeIR!"));
    Serial.println(F("Waiting for signal."));
#endif

#ifdef LCD
    lcd.init();
    lcd.backlight();
    lcd.print(F("Welcome to DecodeIR!"));
    lcd.setCursor(0, 1);
    lcd.print(F("Waiting for signal."));
#endif

    receiver =
#ifdef NON_DEMODULATING_SENSOR
            IrWidgetAggregating::newIrWidgetAggregating(BUFFERSIZE);
#else
            IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
#endif
    receiver->setEndingTimeout(ENDTIMEOUT);
    receiver->setBeginningTimeout(BEGINTIMEOUT);
}

void loop() {
    receiver->receive(); // combines enable, loop, disable

    if (receiver->isEmpty()) {
        //Serial.println(F("timeout"));
        return;
    }

    IrSequence *irSequence = receiver->toIrSequence();
#ifdef NON_DEMODULATING_SENSOR
    frequency_t frequency = ((IrWidgetAggregating*) receiver)->getFrequency();
    DecodeIRClass decoder(*irSequence, frequency);
#else
    DecodeIRClass decoder(*irSequence);
#endif

    delete irSequence;
    if (decoder.getProtocol()[0] == '\0') {
#ifdef USE_SERIAL
        Serial.println(F("** Undecodable **"));
#endif
        return;
    }

#ifdef USE_SERIAL
    Serial.print(decoder.getProtocol());
    Serial.print(" ");
    Serial.print(decoder.getD());
    Serial.print(" ");
    if (decoder.getS() != -1) {
        Serial.print(decoder.getS());
        Serial.print(" ");
    }
    Serial.print(decoder.getF());
    if (decoder.getMiscMessage()[0]) {
        Serial.print(" ");
        Serial.print(decoder.getMiscMessage());
    }
    if (decoder.getErrorMessage()[0]) {
        Serial.print(" ");
        Serial.print(decoder.getErrorMessage());
    }

#ifdef NON_DEMODULATING_SENSOR
    Serial.print(F("; freq="));
    Serial.print(frequency);
    Serial.print(F("Hz"));
#endif

    Serial.println();
#endif

#ifdef LCD
    lcd.clear();
    lcd.home();
    lcd.print(decoder.getProtocol());
    lcd.print(" ");
    lcd.print(decoder.getD());
    lcd.print(" ");
    if (decoder.getS() != -1) {
        lcd.print(decoder.getS());
        lcd.print(" ");
    }
    lcd.print(decoder.getF());
    lcd.setCursor(0, 1);
    if (decoder.getMiscMessage()[0]) {
        lcd.print(decoder.getMiscMessage());
        lcd.print(" ");
    }
    if (decoder.getErrorMessage()[0]) {
        lcd.print(decoder.getErrorMessage());
    }

#if defined(NON_DEMODULATING_SENSOR) && LCD_ROWS > 2
    lcd.setCursor(0, 2);
    lcd.print("Freq=");
    lcd.print(frequency);
    lcd.print("Hz");
#endif //defined(NON_DEMODULATING_SENSOR) && LCD_ROWS > 2

#endif // LCD
}
