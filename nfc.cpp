#include "header.h"

void checkBoardFirmware(Adafruit_PN532 nfc)
{
  uint32_t firmware = nfc.getFirmwareVersion();
  if (!firmware)
  {
    Serial.print("Didn't find PN53x board, halting.");
    while (1)
      Serial.println("No board found, retry"); // halt
  }
}

void printBoardInfo(Adafruit_PN532 nfc)
{
  uint32_t firmware = nfc.getFirmwareVersion();

  Serial.print("Found chip PN5");
  Serial.println((firmware >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((firmware >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((firmware >> 8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A NFC Card ...");
}

void printCardInfo(uint8_t *uid, uint8_t uidLength, Adafruit_PN532 nfc)
{
  nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  Serial.println("Found an ISO14443A card");
  Serial.print("  UID Length: ");
  Serial.print(uidLength, DEC);
  Serial.println(" bytes");
  Serial.print("  UID Value: ");
  nfc.PrintHex(uid, uidLength);
  Serial.println("");
}

bool nfcCardFound(Adafruit_PN532 nfc)
{
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
  uint8_t uidLength;                     // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength) == true)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void printNfcData(uint8_t data[], Adafruit_PN532 nfc)
{
  /*
    NTAG2x3 cards have 39*4 bytes of user pages (156 user bytes),
    starting at page 4 ... larger cards just add pages to the end of
    this range:

    See: http://www.nxp.com/documents/short_data_sheet/NTAG203_SDS.pdf

    TAG Type       PAGES   USER START    USER STOP
    --------       -----   ----------    ---------
    NTAG 203       42      4             39
    NTAG 213       45      4             39
    NTAG 215       135     4             129
    NTAG 216       231     4             225
  */

  uint8_t success;

  // We probably have an NTAG2xx card (though it could be Ultralight as well)
  Serial.println("Seems to be an NTAG2xx tag (7 byte UID)");

  for (uint8_t i = 0; i < 42; i++)
  {
    success = nfc.ntag2xx_ReadPage(i, data);

    // Display the current page number
    Serial.print("PAGE ");
    if (i < 10)
    {
      Serial.print("0");
      Serial.print(i);
    }
    else
    {
      Serial.print(i);
    }
    Serial.print(": ");

    // Display the results, depending on 'success'
    if (success)
    {
      // Dump the page data
      nfc.PrintHexChar(data, 4);
    }
    else
    {
      Serial.println("Unable to read the requested page!");
    }
  }
}

void resetReadAgain()
{
  // Wait a bit before trying again
  Serial.println("\n\nSend a character to scan another tag!");
  Serial.flush();
  while (!Serial.available())
    ;
  while (Serial.available())
  {
    Serial.read();
  }
  Serial.flush();
}

void lookForNfc(Adafruit_PN532 nfc)
{
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
  uint8_t uidLength = 0;                 // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 0))
  {
    Serial.print("NFC Card UID: ");
    nfc.PrintHex(uid, uidLength);
  }
}