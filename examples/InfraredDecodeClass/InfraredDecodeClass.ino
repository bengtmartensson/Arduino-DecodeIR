// This sketch uses the IrReceiverSampler class of Infrared4Arduino
// (can be installed from Arduino library manager).
// It reads IR signals with that class, and sends it
// to DecodeIRClass, which is an OO-encapsulation of DecodeIR.

// It requires a demodulating sensor connected to pin RECEIVE_PIN.

// Optionally, the decode is "printed" on an LCD display using the
// LiquidCrystal_I2C library, version 1.1.2 or later.

// Sketch uses 111722 bytes of program storage space.
// Global variables use 2491 bytes (30%) of dynamic memory.
// Will not run on anything considerably smaller than a Mega2560.

// Define to have output printed to the Serial.
#define USE_SERIAL

// Define to use an LCD display connected over I2C using the
// LiquidCrystal_I2C library version 1.1.2 or later.
#define LCD

#define LCD_I2C_ADDRESS 0x3F
#define LCD_COLUMNS 20
#define LCD_ROWS 4

#define RECEIVE_PIN 5U
#define BUFFERSIZE 400U
#define BAUD 115200
#define ENDTIMEOUT 100U
#define BEGINTIMEOUT 65535U

#include <IrReceiverSampler.h>
#include "DecodeIRClass.h"
#ifdef LCD
#include <LiquidCrystal_I2C.h>
#endif

IrReceiver *receiver;
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

    receiver = IrReceiverSampler::newIrReceiverSampler(BUFFERSIZE, RECEIVE_PIN);
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
    DecodeIRClass decoder(*irSequence);
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
#endif
}
