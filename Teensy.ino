// Teensy 3.2 used, Serial + Keyboard + Mouse + Joystick configuration
#define PRODUCTION (1)

#if PRODUCTION == 1

#include "header.h"

Adafruit_PN532 nfc(SCL0, SDA0);

int16_t degrees_rotated = 0; // Counts degrees rotated (int16 because it can go from -190 to 190)
bool pinALast;               // Last value of pin A, needed to determine direction
bool pinA;                   // Value of pin A, also needed to determine direction

void setup()
{
  Serial.begin(BAUD);
  pinMode(LED_BUILTIN, OUTPUT);

  button_setup();
  encoder_setup();

  // Read Pin, whatever state it's in will reflect the last position
  pinALast = digitalRead(ENCODER_PIN_A);

  Adafruit_PN532 nfc(SCL0, SDA0);

  nfc.begin();

  delay(1000);
  checkBoardFirmware(nfc);
  // printBoardInfo(nfc);
}

void loop()
{
  pinA = digitalRead(ENCODER_PIN_A);

  if (Serial.available())
  {
    handle_command();
  }

  if (is_rotating(pinA, pinALast))
  {
    if (clockwise(pinA))
    { // It's rotation clockwise
      degrees_rotated += PULSE_INCREMENT;
    }
    else
    { // It's rotating counter-clockwise
      degrees_rotated -= PULSE_INCREMENT;
    }

    handle_full_rotation(&degrees_rotated);

    // We set pinA to the value of pinALast
    // In priciple the value of pinALast and pinA are always equal, unless the encoder is rotating
    pinALast = pinA;
  }

  // There isn't a much cleaner way of doing this I'm afraid
  button_conditional(PIN_JOYSTICK_3, 3);
  button_conditional(PIN_JOYSTICK_4, 4);
  button_conditional(PIN_JOYSTICK_5, 5);
  button_conditional(PIN_JOYSTICK_6, 6);

  uint8_t success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
  uint8_t uidLength;                     // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an NTAG203 card.  When one is found 'uid' will be populated with
  // the UID, and uidLength will indicate the size of the UUID (normally 7)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success)
  {
    if (uidLength == 7)
    {
      uint8_t data[32];

      for (uint8_t i = 7; i < 13; i++)
      {
        success = nfc.ntag2xx_ReadPage(i, data);

        if (success)
        {
          String serialData = "";
          uint32_t szPos;

          for (szPos = 0; szPos < 4; szPos++)
          {
            if (data[szPos] > 0x1F && data[szPos] != ESCAPE_CHARACTER)
              Serial.print((char)data[szPos]);
            if (data[szPos] == ESCAPE_CHARACTER)
            {
              Serial.println("");
            }
          }
        }
      }
    }
    delay(1000);
  }
  nfc.reset();
}

#else // Testing codeblock

void setup()
{
}

void loop()
{
}

#endif