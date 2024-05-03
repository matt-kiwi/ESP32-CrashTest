# ESP32 / Arduino crash testing

Experiments on getting platformio / ESP32 / Arduino framework to core dump to flash partition.

## Looks like we need to do one of two things.
## *1 Use IDF and Arduino as a component.
## *2 Build a custom Arduino library 

Other notes;
Edit sdkconfig with ESP-IDF menuconfig
pio run -t menuconfig

Getting LittleFS to work on IDF
https://github.com/joltwallet/esp_littlefs

Some interesting website links
https://github.com/espressif/esp32-arduino-lib-builder

Tasmota complete system / framework based on ESP32
https://github.com/tasmota

Redirect serial /std out logs to network / UDP / MQTT etc
https://github.com/nopnop2002/esp-idf-net-logging


