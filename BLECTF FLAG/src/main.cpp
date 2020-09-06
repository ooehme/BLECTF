#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEAdvertisedDevice.h>
#include "NimBLEBeacon.h"

const NimBLEUUID UUID_AIRFLAG_SERVICE = NimBLEUUID("47be4cef-5f01-49f0-bb29-f86b572293cc");

int scanTime = 4;
BLEScan *pBLEScan;

void processAirflagDevice(BLEAdvertisedDevice *pPlayerDevice);

class AirFlagDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice *advertisedDevice)
  {
    if (advertisedDevice->isAdvertisingService(UUID_AIRFLAG_SERVICE))
    {
      digitalWrite(2, HIGH);
      delay(25);
      digitalWrite(2, LOW);

      processAirflagDevice(advertisedDevice);
    }
  };
};

void processAirflagDevice(BLEAdvertisedDevice *pPlayerDevice)
{
  Serial.println(pPlayerDevice->toString().c_str());
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting AirFlag.");

  pinMode(2, OUTPUT);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new AirFlagDeviceCallbacks());
  pBLEScan->setDuplicateFilter(true);
  pBLEScan->setActiveScan(false);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop()
{
  Serial.println("Scanning...");
  pBLEScan->start(scanTime, false);
  Serial.println("Scan done!");
  pBLEScan->clearResults();
}