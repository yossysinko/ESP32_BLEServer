#include <Arduino.h>
#include <BLEDevice.h>
#include <BLE2902.h>

// =======================
// UUID untuk Service & Characteristic
// =======================
#define SERVICE_UUID1 "370acc92-9c96-4664-98bc-52ca22a19984"
#define SERVICE_UUID2 "43f2e64b-1a47-4f82-9404-256bb1451fe1"
#define CHARACTERISTIC_UUID1 "4fb93a90-6721-4b0e-86cd-866b2b36af24"
#define CHARACTERISTIC_UUID2 "e68ff4e7-dad5-4871-b9ef-28c8c495c5ab"
#define CHARACTERISTIC_UUID3 "d66dfc41-2211-4866-9538-f48f4d0d02c7"

// =======================
// Variabel global & pointer characteristic
// =======================
int value = 0; // Nilai yang akan dikirim secara berkala lewat Notify
BLECharacteristic *pCharacteristic1 = NULL;
BLECharacteristic *pCharacteristic2 = NULL;
BLECharacteristic *pCharacteristic3 = NULL;

// =======================
// Callback untuk koneksi BLE
// =======================
class MyServerCallbacks : public BLEServerCallbacks
{
  // Event ketika client terhubung
  void onConnect(BLEServer *pServer)
  {
    Serial.println("Connected");
  }

  // Event ketika client terputus
  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("Disconnected");
    delay(100);

    // Restart advertising agar perangkat lain bisa connect lagi
    BLEDevice::startAdvertising();
  }
};

void setup()
{
  Serial.begin(115200);

  // =======================
  // Inisialisasi BLE Device
  // =======================
  BLEDevice::init("ESP32_Server_test"); // Nama BLE device yang akan muncul saat scan
  Serial.println("BLE Device Inisialisasi");

  // =======================
  // Membuat BLE Server
  // =======================
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks()); // Pasang callback koneksi

  // =======================
  // Membuat BLE Service
  // =======================
  BLEService *pService1 = pServer->createService(SERVICE_UUID1);
  BLEService *pService2 = pServer->createService(SERVICE_UUID2);

  // =======================
  // Membuat BLE Characteristic
  // =======================
  // Characteristic 1 → READ, WRITE, NOTIFY, WRITE WITHOUT RESPONSE
  pCharacteristic1 =
      pService1->createCharacteristic(
          CHARACTERISTIC_UUID1,
          BLECharacteristic::PROPERTY_READ |
              BLECharacteristic::PROPERTY_WRITE |
              BLECharacteristic::PROPERTY_NOTIFY |
              BLECharacteristic::PROPERTY_WRITE_NR);

  // Characteristic 2 → READ, WRITE, NOTIFY, INDICATE
  pCharacteristic2 =
      pService2->createCharacteristic(
          CHARACTERISTIC_UUID2,
          BLECharacteristic::PROPERTY_READ |
              BLECharacteristic::PROPERTY_WRITE |
              BLECharacteristic::PROPERTY_NOTIFY |
              BLECharacteristic::PROPERTY_INDICATE);

  // Characteristic 3 → READ, WRITE, NOTIFY, INDICATE (di Service1)
  pCharacteristic3 =
      pService1->createCharacteristic(
          CHARACTERISTIC_UUID3,
          BLECharacteristic::PROPERTY_READ |
              BLECharacteristic::PROPERTY_WRITE |
              BLECharacteristic::PROPERTY_NOTIFY |
              BLECharacteristic::PROPERTY_INDICATE);

  // =======================
  // Descriptor untuk Characteristic 1 (wajib untuk Notify/Indicate)
  // =======================
  pCharacteristic1->addDescriptor(new BLE2902());

  // =======================
  // Memulai Service
  // =======================
  pService1->start();
  pService2->start();

  // =======================
  // Mulai Advertising BLE
  // =======================
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID1);
  pAdvertising->addServiceUUID(SERVICE_UUID2);
  pAdvertising->addServiceUUID(SERVICE_UUID1); // (Duplikat, tidak masalah tapi bisa dihapus)

  // Set initial value untuk tiap characteristic
  pCharacteristic1->setValue("Hello World says Neil1");
  pCharacteristic2->setValue("Hello World says Neil2");
  pCharacteristic3->setValue("Hello World says Neil3");

  pAdvertising->start();
  Serial.println("BLE Advertising Inisialisasi");
}

void loop()
{
  // Mengirim data increment ke Characteristic 1 setiap 1 detik
  pCharacteristic1->setValue((uint8_t *)&value, sizeof(value));
  pCharacteristic1->notify(); // Kirim notifikasi ke client

  value++;     // Increment nilai
  delay(1000); // Interval 1 detik
}
