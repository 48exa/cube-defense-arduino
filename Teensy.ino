// Teensy 3.2 used, Serial + Keyboard + Mouse + Joystick configuration
#define PRODUCTION (1)

#if PRODUCTION == 1

#include "header.h"

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

  delay(1000);
}

void loop()
{
  pinA = digitalRead(ENCODER_PIN_A);

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
}

#else // Testing codeblock

void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(300);
}

void loop()
{
  int val = analogRead(A0);
  Serial.println(val);
  delay(100);
}

#endif