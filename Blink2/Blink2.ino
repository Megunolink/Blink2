#include "CommandHandler.h"

const int LEDPin = 13;

struct BlinkConfiguration
{
  uint32_t Interval; // Time between flashing the LED [ms]
  uint32_t Duration; // Time LED remains on [ms]
};

CommandHandler<> SerialCommandHandler;
uint32_t LastBlink; // millis timer when last blink was started.
BlinkConfiguration Settings;

/*
Command handlers
The blink parameters can be changed by sending commands over the serial port. The
Parser receives and dispatches commands to these functions.
*/
void DoSetInterval(CommandParameter &Parameters)
{
  Settings.Interval = Parameters.NextParameterAsInteger();
}

void DoSetDuration(CommandParameter &Parameters)
{
  Settings.Duration = Parameters.NextParameterAsInteger();
}

void DoGetInterval(CommandParameter &Parameters)
{
  Serial.print(F("Interval = "));
  Serial.println(Settings.Interval);
}

void DoGetDuration(CommandParameter &Parameters)
{
  Serial.print(F("Duration = "));
  Serial.println(Settings.Duration);
}


void setup()
{
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);

  SerialCommandHandler.AddCommand(F("SetInterval"), DoSetInterval);
  SerialCommandHandler.AddCommand(F("SetDuration"), DoSetDuration);
  SerialCommandHandler.AddCommand(F("GetInterval"), DoGetInterval);
  SerialCommandHandler.AddCommand(F("GetDuration"), DoGetDuration);

  // Set default values for blink period and duration
  Settings.Duration = 100; // [ms]
  Settings.Interval = 600; // [ms]
}

void loop()
{
  SerialCommandHandler.Process();

  uint32_t uNow = millis();
  if (uNow - LastBlink < Settings.Duration)
    digitalWrite(LEDPin, HIGH);
  else
    digitalWrite(LEDPin, LOW);

  if (uNow - LastBlink > Settings.Interval)
    LastBlink = uNow;
}
