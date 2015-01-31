#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"
#include "data.h"
#include "drop.h"
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

void printDrop(){
    uint8_t padding = random((maxWidth - drop_width)/8);
    uint8_t byteWidth = (uint8_t)((drop_width+7)/8);
    uint8_t drop[((int)padding+byteWidth)*drop_height];
    for(uint8_t j = 0; j < drop_height; j++){
      for(uint8_t i = 0; i < (padding + byteWidth); i++){
        if (i < padding){
          drop[j*((int)padding+byteWidth)+i] = 0x00;
        } else {
          drop[j*((int)padding+byteWidth)+i] = pgm_read_byte(drop_data+((int)j)*byteWidth+i-padding);
        }
      }
    }
    printer.printBitmap(((int)padding)*8 + drop_width, drop_height, drop, false);
}

void loop(){
  double comp = ((double)currLine/maxLines)/((double)currEntry/numEntries);
  //printer.println(comp/2.0);
  if (random(100) > comp*100/2.0 || currEntry >= numEntries){
    printDrop();
  } else {
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[currEntry])));
    currEntry++;
    printer.println(buffer);
    printer.timeoutWait();
  }
  currLine++;
  delay(1000);
}
