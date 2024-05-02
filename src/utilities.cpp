#include "utilities.h"

void array_to_hex(uint8_t *inputArray, char *outputBuffer, unsigned int len){
    unsigned int idx;
    char nib1,nib2;
    for (idx = 0; idx < len; idx++){
        nib1 = (inputArray[idx] >> 4) & 0x0F;
        nib2 = (inputArray[idx] >> 0) & 0x0F;
        outputBuffer[idx*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        outputBuffer[idx*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    outputBuffer[len*2] = '\0';
}


// https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/reset_reason.html
// https://wokwi.com/projects/392195636782398465
uint8_t getResetReason( char reasonText[], uint8_t cpuCore ){
  RESET_REASON reason = rtc_get_reset_reason(cpuCore);
  switch (reason){
    case 1 : sprintf(reasonText,"POWERON_RESET");break;          /**<1, Vbat power on reset*/
    case 3 : sprintf(reasonText,"SW_RESET");break;               /**<3, Software reset digital core*/
    case 4 : sprintf(reasonText,"OWDT_RESET");break;             /**<4, Legacy watch dog reset digital core*/
    case 5 : sprintf(reasonText,"DEEPSLEEP_RESET");break;        /**<5, Deep Sleep reset digital core*/
    case 6 : sprintf(reasonText,"SDIO_RESET");break;             /**<6, Reset by SLC module, reset digital core*/
    case 7 : sprintf(reasonText,"TG0WDT_SYS_RESET");break;       /**<7, Timer Group0 Watch dog reset digital core*/
    case 8 : sprintf(reasonText,"TG1WDT_SYS_RESET");break;       /**<8, Timer Group1 Watch dog reset digital core*/
    case 9 : sprintf(reasonText,"RTCWDT_SYS_RESET");break;       /**<9, RTC Watch dog Reset digital core*/
    case 10 : sprintf(reasonText,"INTRUSION_RESET");break;       /**<10, Instrusion tested to reset CPU*/
    case 11 : sprintf(reasonText,"TGWDT_CPU_RESET");break;       /**<11, Time Group reset CPU*/
    case 12 : sprintf(reasonText,"SW_CPU_RESET");break;          /**<12, Software reset CPU*/
    case 13 : sprintf(reasonText,"RTCWDT_CPU_RESET");break;      /**<13, RTC Watch dog Reset CPU*/
    case 14 : sprintf(reasonText,"EXT_CPU_RESET");break;         /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : sprintf(reasonText,"RTCWDT_BROWN_OUT_RESET");break;/**<15, Reset when the vdd voltage is not stable*/
    case 16 : sprintf(reasonText,"RTCWDT_RTC_RESET");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : sprintf(reasonText,"NO_MEANING");
  }
  return reason;
}


// Very good example of getting partition data directly
// https://wokwi.com/projects/385965135746234369
void printPartitionTable() {
  Serial.printf("ESP32 Partition table:\n");
  Serial.printf("+ ---- | --- | -------- | -------- | ---------------- +\n");
  Serial.printf("| Type | Sub |  Offset  |   Size   |       Label      |\n");
  Serial.printf("| ---- | --- | -------- | -------- | ---------------- |\n");
  esp_partition_iterator_t pi = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (pi != NULL) {
    do {
      const esp_partition_t* p = esp_partition_get(pi);
      Serial.printf("|  %02x  | %02x  | 0x%06X | 0x%06X | %-16s |\r\n", 
        p->type, p->subtype, p->address, p->size, p->label);
    } while (pi == (esp_partition_next(pi)));
  }
  Serial.printf("+ ---- | --- | -------- | -------- | ---------------- +\n");
  uint8_t partitionSha256[32];
  const esp_partition_t* runningPartition = esp_ota_get_running_partition();
  const esp_app_desc_t* appDescription = esp_ota_get_app_description();
  char shaText[17] = {0};
  char idfVersion[32] = {0};
  strcpy(idfVersion,appDescription->idf_ver);
  for( int idx=0; idx<=7; idx++ ) sprintf(shaText+idx*2,"%02x",appDescription->app_elf_sha256[idx]);
  if( esp_partition_get_sha256(runningPartition, partitionSha256) == ESP_OK ){
    Serial.printf("Active partition %s ELF file SHA256: %s IDF version: %s\n\n",runningPartition->label,shaText,idfVersion);
  }



}

void listRootDir(){
  listDir( "/", 5);
}

void listDir(const char *dirname, uint8_t levels) {
  File root = LittleFS.open(dirname);
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.printf( "%50s \n",file.path() );
      if (levels) {
        listDir(file.path(), levels - 1);
      }
    } else {
      time_t lw = file.getLastWrite();
      struct tm *tmstruct = localtime(&lw);
      Serial.printf("%50s %d-%02d-%02d %02d:%02d:%02d %d\n",file.path(),(tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec,file.size() );
    }
    file = root.openNextFile();
    
  }
}
