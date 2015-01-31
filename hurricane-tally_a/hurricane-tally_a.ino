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
int maxLines = 6000;
int interval = 0;//3000;//3 seconds
boolean upsidedown = false;

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

void setup(){
  Serial.begin(9600);
  printer.begin();
  //printer.doubleHeightOn();
  //printer.boldOn();
  if (upsidedown){
    printer.upsideDownOn();
  }
  printer.boldOn();
  printer.doubleHeightOn();
  //printer.doubleWidthOn();
  printer.setSize('L');
  printer.inverseOn();
  printer.justify('C');
  printer.println("Hurricane Watch");
  printer.boldOff();
  printer.doubleHeightOff();
  printer.doubleWidthOff();
  printer.setSize('S');
  printer.inverseOff();
  printer.justify('L');
  printer.println("by Inessah Selditz");
  printer.justify('R');
  printer.println("(pronunc. Quin Kennedy)");
  printer.justify('L');
  for(int i = 0; i < 60; i++){
    printer.println("");
  }
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
          if (upsidedown){
            drop[j*((int)padding+byteWidth)+i] = pgm_read_byte(drop_data+((int)drop_height-1-j)*byteWidth+i-padding);
          } else {
            drop[j*((int)padding+byteWidth)+i] = pgm_read_byte(drop_data+((int)j)*byteWidth+i-padding);
          }
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
    int justified = random(3);
    switch(justified){
      case 0:
        printer.justify('L');
        break;
      case 1:
        printer.justify('C');
        break;
      case 2:
        printer.justify('R');
        break;
    }
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[currEntry])));
    currEntry++;
    printer.println(buffer);
    printer.timeoutWait();
    printer.justify('L');
  }
  if (currLine < maxLines){
    currLine++;
  }
  delay(interval);
}
