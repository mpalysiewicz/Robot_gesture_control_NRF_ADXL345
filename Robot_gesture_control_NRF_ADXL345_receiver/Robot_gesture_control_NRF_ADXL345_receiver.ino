#include <SPI.h>      //SPI library for communicate with the nRF24L01+
#include "RF24.h"     //The main library of the nRF24L01+

//Define packet for the direction (X axis and Y axis)
int data[2];

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
RF24 radio(7,8);

//Create a pipe addresses for the communicate
const uint64_t pipe = 0xE8E8F0F0E1LL;

int fullPowerLeft = 235;
int slowPowerLeft = 110;
int fullPowerRight = 200;
int slowPowerRight = 110;

int PWMApin = 9;
int AIN1pin = 3;
int AIN2pin = 2;
int PWMBpin = 4;
int BIN1pin = 6;
int BIN2pin = 5;

void setup() {
  pinMode(PWMApin, OUTPUT);
  pinMode(AIN1pin, OUTPUT);
  pinMode(AIN2pin, OUTPUT);
  pinMode(PWMBpin, OUTPUT);
  pinMode(BIN1pin, OUTPUT);
  pinMode(BIN2pin, OUTPUT);

  Serial.begin(9600);
  radio.begin();                    //Start the nRF24 communicate            
  radio.openReadingPipe(1, pipe);   //Sets the address of the transmitter to which the program will receive data.
  radio.startListening(); 
}

void loop() {
if (radio.available()){
    radio.read(data, sizeof(data));
    Serial.println("received: ");
    Serial.println(data[0]);
     Serial.println("===============");

    if(data[0] < -50){
      backward();
    }
    if(data[0] > 50){
      
      forward();
    }
    if(data[1] < -50){
      turnRight();
    }
    if(data[1] > 50){
      turnLeft();
    }
    if(data[0] > -30 && data[0] < 30 && data[1] > -30 && data[1] < 30){
      stopMotors();
    }
  }
}

void stopMotors()
{
  digitalWrite(AIN1pin, LOW);
  digitalWrite(AIN2pin, LOW);
  analogWrite(PWMApin, 0);
  digitalWrite(BIN1pin, LOW);
  digitalWrite(BIN2pin, LOW);
  analogWrite(PWMBpin, 0);
}
  
void turnLeft()
{
  digitalWrite(AIN1pin, HIGH);
  digitalWrite(AIN2pin, LOW);
  analogWrite(PWMApin, slowPowerLeft);
  digitalWrite(BIN1pin, LOW);
  digitalWrite(BIN2pin, HIGH);
  analogWrite(PWMBpin, slowPowerRight);
}

void turnRight()
{
  digitalWrite(AIN1pin, LOW);
  digitalWrite(AIN2pin, HIGH);
  analogWrite(PWMApin, slowPowerLeft);
  digitalWrite(BIN1pin, HIGH);
  digitalWrite(BIN2pin, LOW);
  analogWrite(PWMBpin, slowPowerRight);
}

void forward()
{
  digitalWrite(AIN1pin, LOW);
  digitalWrite(AIN2pin, HIGH);
  analogWrite(PWMApin, fullPowerLeft);
  digitalWrite(BIN1pin, LOW);
  digitalWrite(BIN2pin, HIGH);
  analogWrite(PWMBpin, fullPowerRight);
}

void backward()
{
  digitalWrite(AIN1pin, HIGH);
  digitalWrite(AIN2pin, LOW);
  analogWrite(PWMApin, fullPowerLeft);
  digitalWrite(BIN1pin, HIGH);
  digitalWrite(BIN2pin, LOW);
  analogWrite(PWMBpin, fullPowerRight);
}
