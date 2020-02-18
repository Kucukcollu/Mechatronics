/********************************************************************************************************
    Arduino project code for Obstacle Detection Mobile Robot with control of it via motion of body part
                              --- Transmitter Code (Hand Part Code) ---
                                        Ali Aydın KÜÇÜKÇÖLLÜ
                                        Oğuzhan ÜNLÜ
                                        Efe KURDOĞLU
 *******************************************************************************************************/

#include <MPU6050_tockn.h>                          // Adds the MPU6050 library of Acceleraion/Gyro sensor module.
#include <Wire.h>                                   // Adds the I2C library.

#include <SPI.h>                                    // Adds the SPI library to communication with nRF24L01 modules.
#include <nRF24L01.h>                               // Adds the library of nRF24L01 module.
#include <RF24.h>


RF24 radio(8, 9);                                   // Specifies CE and CSN pins of nRF24L01 modules.
const byte address[6] = "00001";                    // Same special address for TX and RX.

struct data{                                        // Creates a variable that structures a selected set of data.
  int xAxis;
  int yAxis;

};
data send_data;

MPU6050 mpu6050(Wire);                              // Creates an object as 'mpu6050'.

void setup() {
  Serial.begin(9600);                               // Starts serial comminication.
  
  Wire.begin();                                     // Starts I2C comminication.
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  radio.begin();                                    // Starts SPI comminication.
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  mpu6050.update();
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.println(mpu6050.getAngleY());


  send_data.xAxis = mpu6050.getAngleX();            // Transmits the data angle x and angle y to mobile robot.
  send_data.yAxis = mpu6050.getAngleY();
  radio.write(&send_data, sizeof(data));
  delay(100);
}
