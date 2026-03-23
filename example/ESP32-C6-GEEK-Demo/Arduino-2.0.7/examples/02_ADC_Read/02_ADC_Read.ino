#include <Arduino.h>

#define ADC1_CHANNEL_0 0  //Define the pin macro 
#define DEV_BL_PIN  6

void setup() {
  analogWrite(DEV_BL_PIN,0);
  Serial.begin(115200);      //The serial port is initially configured
  analogReadResolution(12);  //Set ADC resolution to 12 bits (0-4096)
}

void loop() {
  //Define two variables to hold the original value and the voltage value (millivolts) collected by the ADC
  int analogOriginalValue = 0;
  int analogVoltsValue = 0;

  analogOriginalValue = analogRead(ADC1_CHANNEL_0);         // Read the ADC raw value
  analogVoltsValue = analogReadMilliVolts(ADC1_CHANNEL_0);  // Read ADC voltage values (millivolt range)
  
  // Upload read ADC values:
  Serial.printf("ADC analog value = %d\n",analogOriginalValue);
  Serial.printf("ADC millivolts value = %d mV\n",analogVoltsValue);
  
  delay(3000);
}
