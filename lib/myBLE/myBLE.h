#ifndef myBLE_h
#define myBLE_h

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class myBLE {
  public:
    myBLE(const char* deviceName, const char* serviceUUID, const char* characteristicUUID);
    void begin();
    void kirimData(const char* data);
    
  private:
    BLECharacteristic *pCharacteristic;
    const char* _deviceName;
    const char* _serviceUUID;
    const char* _characteristicUUID;
};

#endif