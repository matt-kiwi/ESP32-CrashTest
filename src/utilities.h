#ifndef UTILITIES_h
#define UTILITIES_h
#include <Arduino.h>
#include "stdint.h"
#include <rom/rtc.h>
#include "FS.h"
#include <LittleFS.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_partition.h"

extern "C" {
#include "esp_ota_ops.h"
#include "esp_image_format.h"
}

#define SHA256_HASH_LEN 8

void array_to_hex(uint8_t *inputArray, char *outputBuffer, unsigned int len);
uint8_t getResetReason( char reasonText[], uint8_t cpuCore );
void printPartitionTable();
void printEconodeBanner();
void listDir(const char *dirname, uint8_t levels);
void listRootDir();

#endif