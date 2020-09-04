#include <Arduino.h>
#include <NimBLEDevice.h>

NimBLEUUID UUID_SERVICEDATA = NimBLEUUID::fromString("47be4cef-5f01-49f0-bb29-f86b572293c3");

void scanEndedCB(NimBLEScanResults results);

static uint32_t scanTime = 4; /** 0 = scan forever */

class AdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks
{
  void onResult(NimBLEAdvertisedDevice *advertisedDevice)
  {
    if (advertisedDevice->haveName())
    {
      //Serial.print("Advertised Device found: ");
      //Serial.println(advertisedDevice->toString().c_str());

      Serial.print("RSSI        :\t");
      Serial.println(advertisedDevice->getRSSI());
      Serial.print("NAME        :\t");
      Serial.println(advertisedDevice->getName().c_str());
      Serial.print("MANUFACTURER:\t");
      Serial.println(advertisedDevice->getManufacturerData().c_str());
      if (advertisedDevice->haveServiceData())
      {
        Serial.print("SERVICEDATA :\t");
        Serial.println(advertisedDevice->getServiceData(UUID_SERVICEDATA).c_str());
      }
    }
  };
};

void scanEndedCB(NimBLEScanResults results)
{
  Serial.println("-----------------");
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting NimBLE Client");

  NimBLEDevice::init("");
  NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_SC);
  NimBLEDevice::setPower(ESP_PWR_LVL_N0); /** +9db */

  NimBLEScan *pScan = NimBLEDevice::getScan();
  pScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
  pScan->setInterval(45);
  pScan->setWindow(15);
  pScan->setActiveScan(true);
  pScan->start(scanTime, scanEndedCB);
}

void loop()
{
  delay(5000);
  NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
}