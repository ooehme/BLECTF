#include <Arduino.h>
#include <NimBLEDevice.h>

void scanEndedCB(NimBLEScanResults results);

static uint32_t scanTime = 4; /** 0 = scan forever */

class AdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks
{
  void onResult(NimBLEAdvertisedDevice *advertisedDevice)
  {
    if (advertisedDevice->haveName())
    {
      Serial.print("Advertised Device found: ");
      Serial.println(advertisedDevice->toString().c_str());
    }
  };
};

void scanEndedCB(NimBLEScanResults results)
{
  Serial.println("Scan Ended");
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
  delay(500);
  NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
}