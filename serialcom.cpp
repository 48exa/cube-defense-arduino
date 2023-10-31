#include "header.h"

void passValuesToComputer()
{
  // construct string from read values
  String sOutput = "";

  // add value analog A0
  int adc0 = random(-127, 127);
  sOutput = sOutput + adc0;

  // add value analog A1
  int adc1 = random(-127, 127);
  sOutput = sOutput + "," + adc1;

  // add value analog A2
  int adc2 = random(-127, 127);
  sOutput = sOutput + "," + adc2;

  // add value analog A3
  int adc3 = random(-127, 127);
  sOutput = sOutput + "," + adc3;

  Serial.println(sOutput);
}

void parseCommands()
{
  // read next byte as command
  delay(1); // a small delay seems necessary to get Serial.available working again?
  if (Serial.available())
  {
    char inByte = Serial.read();
    if (inByte == 'x')
    {
      digitalWrite(LED_BUILTIN, HIGH); // set the LED on
    }
    else if (inByte == 'y')
    {
      digitalWrite(LED_BUILTIN, LOW); // set the LED off
    }                                 // else, ignore this byte
  }
}

void handle_command()
{
  char inByte = Serial.read();

  if (inByte == GIVE_INPUT)
  {
    // pass input values to computer
    passValuesToComputer();
  }
  else if (inByte == ISSUE_COMMAND)
  {
    // parse commands from computer
    parseCommands();
  } // else, ignore this byte
}