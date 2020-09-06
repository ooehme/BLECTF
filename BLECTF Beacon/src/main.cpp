#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEBeacon.h>
#include <NimBLEAdvertising.h>
#include <esp_sleep.h>

#define GPIO_DEEP_SLEEP_DURATION 2

const NimBLEUUID UUID_AIRFLAG_SERVICE = NimBLEUUID("47be4cef-5f01-49f0-bb29-f86b572293cc");

BLEAdvertising *pAdvertising;

RTC_DATA_ATTR char playerName[12];
RTC_DATA_ATTR char teamColor[6];
RTC_DATA_ATTR bool bleActive = false;

void setBeacon()
{
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  oAdvertisementData.setName(teamColor);
  oAdvertisementData.setManufacturerData(playerName);
  oAdvertisementData.setCompleteServices(UUID_AIRFLAG_SERVICE);

  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setAdvertisementType(BLE_HCI_ADV_TYPE_ADV_SCAN_IND);
  pAdvertising->setScanResponse(false);
}

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  BLEDevice::init("teamColor");
  BLEDevice::setPower(ESP_PWR_LVL_N12);

  pAdvertising = BLEDevice::getAdvertising();

  setBeacon();

  pAdvertising->start();
  digitalWrite(2, HIGH);
  delay(2000);
  digitalWrite(2, LOW);
  pAdvertising->stop();
  esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
}

void loop()
{
}