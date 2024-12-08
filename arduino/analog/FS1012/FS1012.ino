/* Analog input, serial output
 
 Reads an analog input pin, performs a simple averaging routine and prints the result to the serial monitor.
 
 The circuit:
 FS1012 Analog Flow Sensor:
   - pin 1: connected to analog pin 0.
   - pin 2: connected to ground.
   - pin 4: connected to +5V.
   - pin 6: connected to ground.
 
 created 3 Jan. 2015
 by MV
*/

// Constants
const int analogInPin0 = A0;       // analog input pin that the flow sensor is attached to
const int sampleAverage = 20;      // number of sensor samples to average    
const int serialRateOutput = 100;  // sample output rate in ms
const int analogSampleDelay = 2;   // sample delay for analog sampling in ms

// Variables
int sensorValue = 0;               // value read from the flow sensor
float sensorAverage = 0.0;         // average sensor value

void setup() 
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 

  // set internal analog reference to 1.1V
  analogReference(INTERNAL);
}

void loop() 
{
  // initialize average sensor value
  sensorAverage = 0.0;
  
  // read and accumulate sensor measurements
  for(int i = 0; i < sampleAverage; i++)
  {
    // read the analog in value:
    sensorValue = analogRead(analogInPin0);
    
    // wait for the analog-to-digital converter 
    // to settle after the last reading:
    delay(analogSampleDelay);
    
    // accumulate sensor data
    sensorAverage = sensorAverage + sensorValue;
  }  

  // calculate sensor average
  sensorAverage = sensorAverage / sampleAverage;
 
  // print the results to the serial monitor:
  Serial.print("FS1012 flow sensor = " ); 
  Serial.println(sensorAverage);     
  
  // wait to ensure at least 100ms between each serial print
  delay(serialRateOutput - (sampleAverage * analogSampleDelay));         
}
