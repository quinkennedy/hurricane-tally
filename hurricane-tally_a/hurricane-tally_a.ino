#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"
#include "data.h"
#include <avr/pgmspace.h>

int printer_RX_Pin = 5; // green
int printer_TX_Pin = 6; // yellow
const int maxWidth = 384;
char buffer[charW];
int i = 0;

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

void setup(){
  Serial.begin(9600);
  printer.begin();
  printer.doubleHeightOn();
  printer.boldOn();
}

void loop(){
  if (i < numEntries){
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i++])));
    printer.println(buffer);
    printer.timeoutWait();
  }
  delay(1000);
}
