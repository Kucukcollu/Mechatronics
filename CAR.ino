/********************************************************************************************************
    Arduino project code for Obstacle Detection Mobile Robot with control of it via motion of body part
                              --- Receiver Code (Mobile Robot Code) ---
                                        Ali Aydın KÜÇÜKÇÖLLÜ
                                        Oğuzhan ÜNLÜ
                                        Efe KURDOĞLU
 *******************************************************************************************************/


#include <SPI.h>                                    // Adds the SPI library to communication with nRF24L01 modules.
#include <nRF24L01.h>                               // Adds the library of nRF24L01 module.
#include <RF24.h>

RF24 radio(8, 9);                                   // Specifies CE and CSN pins of nRF24L01 modules.

const byte address[6] = "00001";                    // Same special address for TX and RX.

int buzzer = A0;

struct data {                                       // Creates a variable that structures a selected set of data.
  int xAxis;
  int yAxis;

};
data receive_data;

const int trigger = 3;
const int echo = 2;

float distance;
float sensor_time;

int motorA1 = 7;                                    // Left side motor.
int motorB1 = 6;

int motorA2 = 5;                                    // Right side motor.
int motorB2 = 4;


void setup() {
  
  Serial.begin(9600);                               // Starts serial comminication.
  
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);

  pinMode(motorA1,OUTPUT);
  pinMode(motorB1,OUTPUT);
  pinMode(motorA2,OUTPUT);
  pinMode(motorB2,OUTPUT);

  radio.begin();                                    // Starts SPI comminication.
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  pinMode(buzzer,OUTPUT);
}

void loop() {
  
  digitalWrite(trigger,HIGH);
  delay(1);
  digitalWrite(trigger,LOW);

  sensor_time = pulseIn(echo,HIGH);                 // Waits for echo signal of HC-SR04 sensor module.
  distance = (sensor_time/2)/28.67;                 // 28.67 ----> Velocity of sound at 30°C.

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < 20){
    analogWrite(buzzer,255);
    delay(200);
    analogWrite(buzzer,0);
  }
  else{
    analogWrite(buzzer,0);
  }
  
  while(radio.available()){                         // Code block to control of mobile robot thanks to motion of hand .
    
    radio.read(&receive_data, sizeof(data));
    
    if(receive_data.xAxis > 50 && distance > 20){
      forward();
    }
    
    else if(receive_data.xAxis < -40){
      backward();
    }
    
    else if(receive_data.yAxis > 45 && distance > 20) {
      left();
    }
    
    else if(receive_data.yAxis < -50 && distance > 20) {
      right();
    }
    
    else {
      keep_calm();
    }
  }
}

void forward(){                                   // Code blocks for motion functions.
  digitalWrite(motorA1,LOW);
  digitalWrite(motorB1,HIGH);
  digitalWrite(motorA2,HIGH);
  digitalWrite(motorB2,LOW);
}

void backward(){
  digitalWrite(motorA1,HIGH);
  digitalWrite(motorB1,LOW);
  digitalWrite(motorA2,LOW);
  digitalWrite(motorB2,HIGH);
}

void right(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorB1,HIGH);
  digitalWrite(motorA2,LOW);
  digitalWrite(motorB2,LOW);
}

void left(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorB1,LOW);
  digitalWrite(motorA2,HIGH);
  digitalWrite(motorB2,LOW);
}

void keep_calm(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorB1,LOW);
  digitalWrite(motorA2,LOW);
  digitalWrite(motorB2,LOW);
}
