#include "myBLE.h"

myBLE::myBLE(const char* deviceName, const char* serviceUUID, const char* characteristicUUID) {
  _deviceName = deviceName;
  _serviceUUID = serviceUUID;
  _characteristicUUID = characteristicUUID;
}

void myBLE::begin() {
  BLEDevice::init(_deviceName);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(_serviceUUID);
  
  pCharacteristic = pService->createCharacteristic(
    _characteristicUUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setValue("Initial Data");
  
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(_serviceUUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
}

void myBLE::kirimData(const char* data) {
  pCharacteristic->setValue(data);
  Serial.print("Data dikirim: ");
  Serial.println(data);
}