//Written by Felix Jochems
#include <Keypad.h>
#include <MFRC522.h>
#include <SPI.h>
#define  RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte nuidPICC[3];
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', '$'}
};
byte rowPins[ROWS] = {7, 6, 5, 4};
byte colPins[COLS] = {14, 15, 16, 17};
String reset = "__RESET__";
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(100);
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
}
void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent())
  {
    if (mfrc522.PICC_ReadCardSerial())
    {
      byte PasID[18];
      byte RekeningID[18];
      byte KlantID[18];
      readBlock(62, PasID);
      readBlock(61, RekeningID);
      readBlock(60, KlantID);
      for (int i = 0; i < 6; i++)
      {
        Serial.write(PasID[i]);
      }
      Serial.write("\n");
      for (int i = 0; i < 6; i++)
      {
        Serial.write(RekeningID[i]);
      }
      Serial.write("\n");
      for (int i = 0; i < 4; i++)
      {
        Serial.write(KlantID[i]);
      }
      Serial.write("\n");
      Serial.println(",NEWUID");
      delay(2000);
      asm volatile ("  jmp 0");
    }
  }
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.print(key);
    Serial.println("KEY");
  }
}
