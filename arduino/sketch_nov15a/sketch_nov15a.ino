/*
  L'obiettivo di questo progetto è creare il software di misura che permetta di 
  utilizzare una coppia di trasduttori di flusso (aria) ed una pompa di aria.

  Questo programma firmware sarà installato su un Arduino Uno. Il codice serve a 
  programmare il sensore Renesas FS2012 e per leggere l'output del sensore Renesas FS1012.
  Un estremo di quest'ultimo sarà collegato ad una pompa
  d'aria EAD NEO-I, che avrà il compito di generare un flusso di aria. La pompa
  d'aria è alimentata da un generatore di tensione Agilent E3631A. Quest'ultimo
  dovrà essere programmato da remoto con un programma LABView.
  L'altro estremo del sensore FS2012 sarà collegato ad un secondo sensore di flusso; in questo
  caso il sensore, Renesas FS1012, ha un'uscita analogica. 
  Mentre il FS2012 è già tarato, il sensore FS1012 dovrà essere tarato attraverso una procedura
  di calibrazione.
  Infine uno strumento virtuale sarà creato per acquisire le misure ottenute dai sue sensori.

*/

#include <I2Cdev.h> //I2C library
#include <Wire.h> //
#include <Arduino.h>

#define SDA A4 //Arduino UNO settings
#define CLK A5
#define Vout A0
#define FS2012_ADDRESS 0x07 // see datasheet 

unsigned long rxcount; 
uint16_t data = 0; //digital data from FS2012
float flow; //flow after convertion
// Constants
const int analogInPin = A1;       // analog input pin that the flow sensor is attached to
const int sampleAverage = 20;      // number of sensor samples to average    
const int serialRateOutput = 100;  // sample output rate in ms
const int analogSampleDelay = 2;   // sample delay for analog sampling in ms
// Variables
int sensorValue = 0;               // value read from the flow sensor
float sensorAverage = 0.0;         // average sensor value
unsigned long delayMs = 400;

void digital_flow_sensor(){
    Wire.beginTransmission(FS2012_ADDRESS); // "Hey, SENSOR @ Address! Message for you"
    Wire.write(1);  // send a bit asking for register one, the data register (as specified by the pdf)
    Wire.endTransmission(); // "Thanks, goodbye..."
    Wire.requestFrom(FS2012_ADDRESS, 2);
    while(Wire.available() == 0);
    byte a = Wire.read(); // first received byte stored here ....Example bytes one: 00011001 10000000
    //delay(5);
    byte b = Wire.read(); // second received byte stored here ....Example bytes two: 11100111 00000000
    //delay(5);
	  data = (a << 8) | b;
    flow = data / 1000.0; // (Standard Liters per minute)
}

void analogue_flow_sensor(){
  // initialize average sensor value
  sensorAverage = 0.0;
  
  // read and accumulate sensor measurements
  for(int i = 0; i < sampleAverage; i++)
  {
    // read the analog in value:
    sensorValue = analogRead(analogInPin);
    
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



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); 
  // set internal analog reference to 1.1V
  analogReference(INTERNAL);

  Wire.begin(); 
  delay(500);
}

void loop() { 
  if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read the command until newline
        serialEvent(command); // Process the received command
    }
  digital_flow_sensor(); //Renesas FS2012
  delay(delayMs);
  //analogue_flow_sensor(); //Renesas FS1012
  
  Serial.println(flow, 5); //print flow measured from FS2012

  //Serial.print(sensorAverage); //print flow measured from FS2012
  //Serial.print('  ');
} 

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent(String command) {
    command.trim(); // Remove any trailing or leading spaces or newline characters

    // Command to set sampling frequency: "SF:<frequency>"
    if (command.startsWith("SF:")) {
        String freqStr = command.substring(3); // Extract the frequency part
        float newFreq = freqStr.toFloat(); // Convert to float

        if (newFreq > 0) { // Ensure frequency is positive
            delayMs = (unsigned long)(1000.0 / newFreq); // Calculate delay in ms
        }
    }
}


