#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define RX_CHARACTERISTIC_UUID "12ee6f51-021d-438f-8094-bf5c5b36eab9"
#define TX_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic_RX) { // Callback funktion som gør at der kan læses en værdi som sendes af BLE-client.
      String value = pCharacteristic_RX->getValue().c_str();
		
      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
	  
    }
};

BLECharacteristic *pCharacteristic_TX; 
BLECharacteristic *pCharacteristic_RX;

void setup() {
Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("ESP32_Master"); // opret en enhed og giver enheden navnet ESP32_Master
  BLEServer *pServer = BLEDevice::createServer(); // sætter enheden til at være en server
  BLEService *pService = pServer->createService(SERVICE_UUID);// opret en tjeneste med egenskaberne
  pCharacteristic_TX = pService->createCharacteristic(
                                         TX_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY 
                                       );
  pCharacteristic_RX = pService->createCharacteristic(
                                         RX_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                                       );                                     
 
  pCharacteristic_RX->setCallbacks(new MyCallbacks());
  

  // Create a BLE Descriptor
  pCharacteristic_RX->addDescriptor(new BLE2902());
  pCharacteristic_TX->addDescriptor(new BLE2902());
  pService->start(); // start tjenesten
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising(); 
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising(); // start annoncering, så andre BLE-enheder kan scanne og finde denne BLE-enhed}

  pCharacteristic_TX->setValue("Hello World");
  pCharacteristic_TX->notify();

};

void loop() {
  // put your main code here, to run repeatedly:
}