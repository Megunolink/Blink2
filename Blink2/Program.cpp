#include <Arduino.h>
#include "Libraries/CommandParser/CommandParser.h"

const int LEDPin = 13;

struct BlinkConfiguration 
{
  uint32_t Interval; // Time between flashing the LED [ms]
  uint32_t Duration; // Time LED remains on [ms]
};

MLP::CommandParser<> Parser;
uint32_t LastBlink; // millis timer when last blink was started. 
BlinkConfiguration Settings;

/* 
Command handlers
The blink parameters can be changed by sending commands over the serial port. The
Parser receives and dispatches commands to these functions. 
*/
void DoSetInterval(MLP::CommandParameter &Parameter)
{
  const char *pchValue;

  pchValue = Parameter.NextParameter();
  if (pchValue == NULL)
  {
    Serial.println(F("Set interval missing parameter"));
    return;
  }

  Settings.Interval = atoi(pchValue);
}

void DoSetDuration(MLP::CommandParameter &Parameter)
{
  const char *pchValue;

  pchValue = Parameter.NextParameter();
  if (pchValue == NULL)
  {
    Serial.println(F("Set duration missing parameter"));
    return;
  }

  Settings.Duration = atoi(pchValue);
}

void DoGetInterval(MLP::CommandParameter &Parameter)
{
  Serial.print(F("Interval = "));
  Serial.println(Settings.Interval);
}

void DoGetDuration(MLP::CommandParameter &Parameter)
{
  Serial.print(F("Duration = "));
  Serial.println(Settings.Duration);
}

void DoUnknown()
{
  Serial.println("I don't understand!");
}

void setup()
{
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);

  Parser.AddCommand(F("SetInterval"), DoSetInterval);
  Parser.AddCommand(F("SetDuration"), DoSetDuration);
  Parser.AddCommand(F("GetInterval"), DoGetInterval);
  Parser.AddCommand(F("GetDuration"), DoGetDuration);
  Parser.SetDefaultHandler(DoUnknown);

  // Set default values for blink period and duration
  Settings.Duration = 100; // [ms]
  Settings.Interval = 600; // [ms]
}

void loop()
{
  Parser.Process();

  uint32_t uNow = millis();
  if (uNow - LastBlink < Settings.Duration)
    digitalWrite(LEDPin, HIGH);
  else
    digitalWrite(LEDPin, LOW);

  if (uNow - LastBlink > Settings.Interval)
    LastBlink = uNow;
}