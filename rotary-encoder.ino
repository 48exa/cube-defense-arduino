// I use specific int sizes for better memory management
// u_int8_t is 1 byte, u_int16_t is 2 bytes, 32 is 4 and 64 is 8
// the maximum number size scales as following, going from smallest (int8_t to int64_t):
// 255, 65536, 4294967296, 18446744073709600000
// [u_] means unsigned, meaning the int can't go below 0
// for example, [int8_t] can store a value between -127 and 127, [u_int8_t] can store a value between 0 and 255.
// the maximum value of a [u_int(x)_t] is half of the value of that of a [int(x)_t]
// this is because instead of counting only positive numbers, unsigned int's also can count negative numbers so they split them in half
// pin values have boolean values as they never have any other value than 0 or 1
// constants are made constants to indicate that they do not change in the code, it is for readability, not performance

/*
 *  JOYSTICK CONFIGURATION:
 *  BUTTON 1: ROTARY ENCODER CLOCKWISE
 *  BUTTON 2: ROTARY ENCODER COUNTER-CLOCKWISE
 *  BUTTON 3: PIN 0 HIGH
 *  BUTTON 4: PIN 1 HIGH
 *  BUTTON 5: PIN 2 HIGH
 *  BUTTON 6: PIN 3 HIGH
 */

#define degrees_per_pulse 12;     // Total amount of degrees per pulse (KY-040 has 30 pulses per full rotation, 360 / 30 = 12)
const u_int8_t _EncoderPinA = 11; // Connected to CLK on KY-040
const u_int8_t _EncoderPinB = 12; // Connected to DT on KY-040
const u_int8_t _pin0 = 0;         // Connected to pin 00, is be a button
const u_int8_t _pin1 = 1;         // Connected to pin 01, is be a button
const u_int8_t _pin2 = 2;         // Connected to pin 02, is be a button
const u_int8_t _pin3 = 3;         // Connected to pin 03, is be a button
int16_t degrees_rotated = 0;      // Counts degrees rotated (int16 because it can go from -190 to 190)
bool pinALast;                    // Last value of pin A, needed to determine direction
bool pinA;                        // Value of pin A, also needed to determine direction

void setup()
{
  // Rotary encoder pins
  pinMode(_EncoderPinA, INPUT);
  pinMode(_EncoderPinB, INPUT);

  // Contact pins
  pinMode(_pin0, INPUT_PULLDOWN);
  pinMode(_pin1, INPUT_PULLDOWN);
  pinMode(_pin2, INPUT_PULLDOWN);
  pinMode(_pin3, INPUT_PULLDOWN);

  // Read Pin, whatever state it's in will reflect the last position
  pinALast = digitalRead(_EncoderPinA);
  Serial.begin(9600);
}
void loop()
{
  pinA = digitalRead(_EncoderPinA);

  if (pinA != pinALast)
  { // This means the knob is rotating
    // If the knob is rotating, we need to determine direction
    // We do that by reading pin B.

    if (digitalRead(_EncoderPinB) != pinA)
    { // This means pin A Changed first - It's rotation clockwise
      degrees_rotated += degrees_per_pulse;
    }
    else
    { // This means pin B changed first - It's rotating counter-clockwise
      degrees_rotated -= degrees_per_pulse;
    }

    switch (degrees_rotated)
    {
      // Commenting out the block for 90 degrees as the current rotary encoder
      // I am using doesn't actually get to 90 degrees as 360 is devisable by 4.
      /*
      case 90: // The encoder has rotated 90 degrees clockwise
        Serial.println("KY-040 rotated 90 degrees clockwise!");

        Joystick.button(1, true);
        delay(100);
        Joystick.button(1, false);

        break;
      case -90: // The encoder has rotated 90 degrees counter-clockwise
        Serial.println("KY-040 90 degrees counter-clockwise!");

        Joystick.button(2, true);
        delay(100);
        Joystick.button(2, false);

        break;
      */
    case 180: // The encoder has rotated 180 degrees clockwise
      Serial.println("You rotated 180 degrees clockwise!!!");
      degrees_rotated = 0;

      Joystick.button(1, true);
      delay(100);
      Joystick.button(1, false);

      break;
    case -180: // The encoder has rotated 180 degrees counter-clockwise
      Serial.println("You rotated 180 degrees counter-clockwise!!!");
      degrees_rotated = 0;

      Joystick.button(2, true);
      delay(100);
      Joystick.button(2, false);

      break;
    default: // Nothing happened so we do nothing
      break;
    }
    // The reason why we say the encoder has rotated 90 degrees when it has rotated 84 is because
    // the encoder I am using sends 30 pulses per 360 degree rotation.
    // Because 30 is not devisable by 4 without fractions I can't reset the degrees_rotated per 90 degrees.
    // But because 30 is devisable by 2 without fractions I reset the counter every 180 degrees instead.

    Serial.print("Encoder Position (deg): ");
    Serial.println(degrees_rotated);
  }
  // We set pinA to the value of pinALast
  // In priciple the value of pinALast and pinA are always opposite
  pinALast = pinA;

  button_conditional(_pin0, 3);
  button_conditional(_pin1, 4);
  button_conditional(_pin2, 5);
  button_conditional(_pin3, 6);
}

/*
  ###############################################################################################################
  Just a divider because I keep losing track of where loop() is lol
  ###############################################################################################################
*/

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
