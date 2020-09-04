#include <Arduino.h>
#include <NimBLEDevice.h>

static NimBLEServer *pServer;
const char *UUID_SERVICE = "47be4cef-5f01-49f0-bb29-f86b572293cc";
const char *UUID_SERVICEDATA = "47be4cef-5f01-49f0-bb29-f86b572293c3";
const char *UUID_CHARACT_PLAYERNAME = "0f0e9fd8-5969-4351-8fb5-13c83cdca6cd";
const char *UUID_CHARACT_TEAMNAME = "300f9c95-3bb0-40cb-a9b2-361e566c43cc";
const char *UUID_CHARACT_TEAMCOLOR = "570fc528-12ac-4d88-8704-e8c3b60903f8";

const struct teamcolorids
{
   int RED = 0;
   int GREEN = 1;
   int BLUE = 2;
   int YELLOW = 3;
   int PINK = 4;
   int WHITE = 5;
} TEAMCOLORIDS;

String playerName = "Player 01";
int teamColor = TEAMCOLORIDS.RED;

String generateUUID(String playerId, int teamColor);
void reverse(char str[], int size, char rev[]);
void hexCon(char str[], char result[][5]);

void setup()
{
   Serial.begin(115200);
   Serial.println("");

   NimBLEDevice::init("Player");
   NimBLEDevice::setPower(ESP_PWR_LVL_N0); /** ESP_PWR_LVL_P9 +9db */
   NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_SC);

   pServer = NimBLEDevice::createServer();

   NimBLEService *pPlayerService = pServer->createService(UUID_SERVICE);
   NimBLEService *pServiceData = pServer->createService(UUID_SERVICEDATA);
   NimBLECharacteristic *pCharacteristicPlayername = pPlayerService->createCharacteristic(UUID_CHARACT_PLAYERNAME, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::BROADCAST);
   pCharacteristicPlayername->setValue("Player 01");
   NimBLECharacteristic *pCharacteristicTeamcolor = pPlayerService->createCharacteristic(UUID_CHARACT_TEAMCOLOR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::BROADCAST);
   pCharacteristicTeamcolor->setValue(TEAMCOLORIDS.RED);
   NimBLECharacteristic *pCharacteristicTeamname = pPlayerService->createCharacteristic(UUID_CHARACT_TEAMNAME, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::BROADCAST);
   pCharacteristicTeamname->setValue("Intim");
   pPlayerService->start();

   NimBLEAdvertisementData oAdvertisementData = NimBLEAdvertisementData();
   oAdvertisementData.setShortName("AFP");
   oAdvertisementData.setName("AFPlayer");
   String manufacturerName = "AirFlag";
   oAdvertisementData.setManufacturerData(manufacturerName.c_str());
   String data = "shockbase,3,rattikarls";
   oAdvertisementData.setServiceData(pServiceData->getUUID(), data.c_str());

   NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
   pAdvertising->addServiceUUID(pPlayerService->getUUID());
   pAdvertising->setAdvertisementData(oAdvertisementData);

   pAdvertising->setScanResponse(true);
   pAdvertising->start();

   Serial.println("Advertising Started");
}

void loop()
{
   delay(2000);
}