#include <Arduino.h>
#include <BLEDevice.h>
#include <BLE2902.h>

#define SERVICE_UUID1 "370acc92-9c96-4664-98bc-52ca22a19984"
#define SERVICE_UUID2 "43f2e64b-1a47-4f82-9404-256bb1451fe1"
#define CHARACTERISTIC_UUID1 "4fb93a90-6721-4b0e-86cd-866b2b36af24"
#define CHARACTERISTIC_UUID2 "e68ff4e7-dad5-4871-b9ef-28c8c495c5ab"

int value = 0;
BLECharacteristic *pCharacteristic1 = NULL;
BLECharacteristic *pCharacteristic2 = NULL;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Connected");
  }
  void onDisconnect(BLEServer* pServer) {
    Serial.println("Disconnected");
    delay(2000);
    BLEDevice::startAdvertising();
  }
};

void setup() {
  Serial.begin(115200);

  //Inisialisasi BLE Device
  BLEDevice::init("ESP32_Server_test");
  Serial.println("BLE Device Inisialisasi");

  //Membuat BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  //Membuat BLE Service
  BLEService *pService1 = pServer->createService(SERVICE_UUID1);
  BLEService *pService2 = pServer->createService(SERVICE_UUID2);

  //Membuat BLE Characteristic
  pCharacteristic1 = 
  pService1->createCharacteristic(CHARACTERISTIC_UUID1, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR);
  pCharacteristic2 = 
  pService2->createCharacteristic(CHARACTERISTIC_UUID2, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);

  //Membuat BLE Descriptor
  pCharacteristic1->addDescriptor(new BLE2902());

  //Memulai BLE Service
  pService1->start();
  pService2->start();

  //Memulai BLE Advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID1);
  pAdvertising->addServiceUUID(SERVICE_UUID2);
  pCharacteristic1->setValue("Hello World says Neil1");
  pCharacteristic2->setValue("Hello World says Neil2");
  pAdvertising->start();
  Serial.println("BLE Advertising Inisialisasi");
}
void loop() {
    pCharacteristic1->setValue((uint8_t *)&value, sizeof(value)); 
    pCharacteristic1->notify();
    value++;
    delay(1000);
}