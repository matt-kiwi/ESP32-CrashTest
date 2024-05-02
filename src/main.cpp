#include <Arduino.h>
#include <Preferences.h>
#include "utilities.h"
#include <LittleFS.h>
#include "FS.h"
#define FIRMWARE_VERSION "v@1.0.1"

// LittleFS see notes here on using with IDF
// https://github.com/Jason2866/Arduino_IDF_LittleFS/tree/main


void crash_me_hard() {
	//provoke crash through writing to a nullpointer
	volatile uint32_t* aPtr = (uint32_t*) 0x00000000;
	*aPtr = 0x1234567; //goodnight
}

void processSerial(){
  char inChar = Serial.read();
  // Serial.printf("we got:%i\n",inChar);
  if( inChar == 'c' || inChar == 'C'){
    crash_me_hard();
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  if(!LittleFS.begin(true)){
    Serial.println("An Error has occurred while mounting LittleFS");
  }
  
  printPartitionTable();
  listRootDir();
  Serial.printf("\n press C to cause crash\n");
}

void loop() {
  if( Serial.available()> 0 ) processSerial();
  delay(10);
}
