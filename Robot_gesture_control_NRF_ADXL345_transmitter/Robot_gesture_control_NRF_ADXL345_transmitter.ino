//Mert Arduino and Tech YouTube Channel -- https://goo.gl/ivcZhW

//Add the necessary libraries
#include <SPI.h>        //SPI library for communicate with the nRF24L01+
#include "nRF24L01.h"
#include "RF24.h"       //The main library of the nRF24L01+: https://github.com/nRF24/RF24
#include "Wire.h"       //For communicate
#include <ADXL345.h>    // 

ADXL345 accelerometer;


//Define packet for the direction (X axis and Y axis)
int data[2];

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected.
RF24 radio(7,8);

//Create a pipe addresses for the communicate                                    
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void){
  Serial.begin(9600);
  Wire.begin();
  
  // Initialize ADXL345
  Serial.println("Initialize ADXL345");

  radio.begin();                 //Start the nRF24 communicate     
  radio.openWritingPipe(pipe);   //Sets the address of the receiver to which the program will send data.
  

    if (!accelerometer.begin())
  {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    delay(500);
  }

  // Set measurement range
  // +/-  2G: ADXL345_RANGE_2G
  // +/-  4G: ADXL345_RANGE_4G
  // +/-  8G: ADXL345_RANGE_8G
  // +/- 16G: ADXL345_RANGE_16G
  accelerometer.setRange(ADXL345_RANGE_16G);

}


void loop(void){

  // Read normalized values
  Vector norm = accelerometer.readNormalize();
  
  // Low Pass Filter to smooth out data. 0.1 - 0.9
  Vector filtered = accelerometer.lowPassFilter(norm, 0.5);

    // Calculate Pitch & Roll (Low Pass Filter)
  int fpitch = -(atan2(filtered.XAxis, sqrt(filtered.YAxis*filtered.YAxis + filtered.ZAxis*filtered.ZAxis))*180.0)/M_PI;
  int froll  = (atan2(filtered.YAxis, filtered.ZAxis)*180.0)/M_PI;
  
  //In two-way control, the X axis (data [0]) of the ADXL345 allows the robot to move forward and backward. 
  //Y axis (data [0]) allows the robot to right and left turn.
  data[0] = fpitch; //Send X axis data
  data[1] = froll;  //Send Y axis data
  radio.write(data, sizeof(data));

        
  //Serial.print(" (filter)Pitch = ");
  //Serial.print(fpitch);
  //Serial.print(" (filter)Roll = ");
  //Serial.print(froll);
  //Serial.println();

  delay(100);
}
