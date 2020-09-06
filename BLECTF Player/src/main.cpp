#include <Arduino.h>
#include <NimBLEDevice.h>

static NimBLEServer *pServer;

const NimBLEUUID UUID_AIRFLAG_SERVICE = NimBLEUUID::fromString("47be4cef-5f01-49f0-bb29-f86b572293cc");

const struct teamcolorids
{
   int RED = 0;
   int GREEN = 1;
   int BLUE = 2;
   int YELLOW = 3;
   int PINK = 4;
   int WHITE = 5;
} TEAMCOLORS;

std::string playerName = "Player 01";
std::string teamColor = "BLUE";
std::string teamName = "rattikarls";

void setup()
{
   Serial.begin(115200);
   Serial.println("Starting AirFlag");

   NimBLEDevice::init(teamColor);
   NimBLEDevice::setPower(ESP_PWR_LVL_N0);

   pServer = NimBLEDevice::createServer();

   NimBLEService *pPlayerService = pServer->createService(UUID_AIRFLAG_SERVICE);

   pPlayerService->start();

   NimBLEAdvertisementData oAdvertisementData = NimBLEAdvertisementData();
   NimBLEAdvertisementData oScanResponseData = NimBLEAdvertisementData();

   oAdvertisementData.setFlags(0 | BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP);
   oAdvertisementData.setShortName(playerName);
   oAdvertisementData.setName(teamColor);
   oAdvertisementData.setManufacturerData(teamName);

   NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
   pAdvertising->addServiceUUID(UUID_AIRFLAG_SERVICE);
   pAdvertising->setAdvertisementType(BLE_HCI_ADV_TYPE_ADV_SCAN_IND);
   pAdvertising->setAdvertisementData(oAdvertisementData);
   pAdvertising->setScanResponse(true);

   pAdvertising->start();

   Serial.println("Advertising Started");
}

void loop()
{
   delay(2000);
}
