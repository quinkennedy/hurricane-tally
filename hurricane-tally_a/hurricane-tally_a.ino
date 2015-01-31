#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"
#include "data.h"
#include <avr/pgmspace.h>

int printer_RX_Pin = 5; // green
int printer_TX_Pin = 6; // yellow
const int maxWidth = 384;
char buffer[charW];
int currEntry = 0;
int currLine = 0;
int maxLines = 1700;

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

void setup(){
  Serial.begin(9600);
  printer.begin();
  //printer.doubleHeightOn();
  //printer.boldOn();
}

void loop(){
  double comp = ((double)currLine/maxLines)/((double)currEntry/numEntries);
  printer.println(comp/2.0);
  if (random(100) > comp*100/2.0 || currEntry >= numEntries){
    printer.println("/drip");
  } else {
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[currEntry])));
    currEntry++;
    printer.println(buffer);
    printer.timeoutWait();
  }
  currLine++;
  delay(1000);
}
