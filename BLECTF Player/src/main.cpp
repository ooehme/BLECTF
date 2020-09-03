#include <Arduino.h>
#include <NimBLEDevice.h>

static NimBLEServer *pServer;

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

   String uuid = generateUUID(playerName, teamColor);

   char uuidBuf[37];
   uuid.toCharArray(uuidBuf, 37);
   
   Serial.print("uuid2 :\t\t");
   Serial.println(uuidBuf);

   NimBLEDevice::init("Player");
   NimBLEDevice::setPower(ESP_PWR_LVL_N0); /** ESP_PWR_LVL_P9 +9db */
   NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_SC);

   pServer = NimBLEDevice::createServer();

   NimBLEService *pPlayerService = pServer->createService(uuidBuf);
   pPlayerService->start();

   NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
   pAdvertising->addServiceUUID(pPlayerService->getUUID());

   pAdvertising->setScanResponse(false);
   pAdvertising->start();

   Serial.println("Advertising Started");
}

void loop()
{
   delay(2000);
}

String generateUUID(String playerId, int teamColor)
{
   String payload;
   String uuid;
   char teamColorBuf[5];
   itoa(teamColor, teamColorBuf, 10);

   playerId += ",";
   playerId += teamColorBuf;

   char str[playerId.length() + 1];
   playerId.toCharArray(str, playerId.length() + 1);

   char result[strlen(str)][5];
   hexCon(str, result);

   for (int i = 0; i < strlen(str); i++)
   {
      payload += result[i];
   }

   while(payload.length() < 32)
   {
      payload += "0";
   }

   Serial.print("payload :\t");
   Serial.println(payload);

   uuid = payload.substring(0, 8) + "-" + payload.substring(8, 12) + "-" + payload.substring(12, 16) + "-" + payload.substring(16, 20) + "-" + payload.substring(20);
   Serial.print("uuid1 :\t\t");
   Serial.println(uuid);

   return uuid;
}

void hexCon(char str[], char result[][5])
{
   int i, check, rem = 0;
   char res[20], res2[20];
   int len = 0;
   char temp;

   for (i = 0; i < strlen(str); i++)
   {
      len = 0;
      check = str[i];
      while (check > 0)
      {
         rem = check % 16;
         switch (rem)
         {
         case 10:
            temp = 'A';
            break;
         case 11:
            temp = 'B';
            break;
         case 12:
            temp = 'C';
            break;
         case 13:
            temp = 'D';
            break;
         case 14:
            temp = 'E';
            break;
         case 15:
            temp = 'F';
            break;
         default:
            temp = rem + '0';
         }
         res[len] = temp;
         check = check / 16;
         len++;
      }

      reverse(res, len, res2); //reversing the digits
      res2[len] = '\0';        //adding null character at the end of string
      strcpy(result[i], res2); //copying all data to result array
   }
}

void reverse(char str[], int size, char rev[])
{
   int i = 0, j = 0;
   for (i = size - 1, j = 0; i >= 0; i--, j++)
   {
      rev[j] = str[i];
   }
}