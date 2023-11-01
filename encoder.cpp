#include "header.h"

void encoder_setup()
{
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
}

bool is_rotating(bool pin_current, bool pin_previous)
{
  if (pin_current != pin_previous)
  { // This means the knob is rotating
    return true;
  }
  else
  { // This means it... isn't
    return false;
  }
}

bool clockwise(bool pin_current)
{ // oly
  if (digitalRead(ENCODER_PIN_B) != pin_current)
  { // If the B pin is not equal to the A pin it means the encoder is turning clockwise
    // if you crave more knowledge about why exactly read the datasheet linked below
    // https://www.handsontec.com/dataspecs/module/Rotary%20Encoder.pdf
    return true;
  }
  else
  { // If the encoder isn't turning clockwise it must be turning counter-clockwise
    // This function is only used AFTER a check for if the encoder is rotating, that
    // why I'm not accounting for cases where it isn't
    return false;
  }
}

int16_t handle_full_rotation(int16_t degrees)
{
  switch (degrees)
  {
  case RESET_PULSES: // The encoder has rotated 180 degrees clockwise
    degrees = 0;
    pulse_button(1, 100);
    break;
  case -RESET_PULSES: // The encoder has rotated 180 degrees counter-clockwise
    degrees = 0;
    pulse_button(2, 100);
    break;
  default: // Nothing happened so we do nothing
    break;
  }

  Serial.print("Encoder Position (deg): ");
  Serial.println(degrees);
  return degrees;
}