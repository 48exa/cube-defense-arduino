#include "header.h"

void button_setup()
{
  pinMode(PIN_JOYSTICK_3, INPUT_PULLDOWN);
  pinMode(PIN_JOYSTICK_4, INPUT_PULLDOWN);
  pinMode(PIN_JOYSTICK_5, INPUT_PULLDOWN);
  pinMode(PIN_JOYSTICK_6, INPUT_PULLDOWN);
}

void pulse_button(int8_t button, int32_t millis)
{
  Joystick.button(button, true);
  delay(millis);
  Joystick.button(button, false);
}

void button_conditional(u_int8_t pin, u_int8_t button)
{
  if (digitalRead(pin) == HIGH)
  {
    Joystick.button(button, true);
  }
  else
  {
    Joystick.button(button, false);
  }
}