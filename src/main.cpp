#include <Arduino.h>
#include <Preferences.h>
#include "utilities.h"
#include <LittleFS.h>
#include "FS.h"
#include "esp_partition.h"
#include "esp_core_dump.h"
#define FIRMWARE_VERSION "v@1.0.1"
#define BUFFER_SIZE 4096
#define PARTITION_NAME "coredump"

// Borrowed from https://github.com/ddomnik/coredump_example


// LittleFS see notes here on using with IDF
// https://github.com/Jason2866/Arduino_IDF_LittleFS/tree/main



const esp_partition_t* findCoreDumpPartition()
{
    esp_partition_type_t p_type = ESP_PARTITION_TYPE_DATA;
    esp_partition_subtype_t p_subtype = ESP_PARTITION_SUBTYPE_DATA_COREDUMP;
    const char *label = PARTITION_NAME;

    return esp_partition_find_first(p_type, p_subtype, label);
}

void displayCrashInfo(){
  //
  const esp_partition_t* partition = findCoreDumpPartition();
  // esp_core_dump_flash_check();
}

void displayChipInfo(){
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  Serial.printf("This chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  Serial.printf("silicon revision %d, ", chip_info.revision);
}

void crash_me_hard() {
	//provoke crash through writing to a nullpointer
	// volatile uint32_t* aPtr = (uint32_t*) 0x00000000;
	// *aPtr = 0x1234567; //goodnight
  assert(0);
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
  displayChipInfo();
  Serial.printf("\n press C to cause crash\n");
}

void loop() {
  if( Serial.available()> 0 ) processSerial();
  delay(10);
}
