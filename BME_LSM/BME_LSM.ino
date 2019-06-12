//Serial output of LSM9DS1 and BME280
#include <Wire.h>
#include <SparkFunLSM9DS1.h>
#include "SparkFunBME280.h"


LSM9DS1 imu;
BME280 bme;

//I2C setup
#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B

//Output
#define PRINT_CALCULATED
#define PRINT_SPEED 250
//static unsigned long lastPrint = 0;
//#define DECLINATION -3.88 //Set for Milwaukee, WI

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  Wire.begin();
  
  if (!imu.begin()){
    Serial.println("IMU did not respond.");
    while (1);
  }
  if (bme.beginI2C() == false){
    Serial.println("BME did not respond.");
    while (1);
  }
}

void loop() {
  Serial.println();
  Serial.print("Temp: ");
  Serial.println(bme.readTempC(), 2);

  Serial.print("Pressure: ");
  Serial.println(bme.readFloatPressure(), 0);

  printGyro();
  printAccel();
  printMag();
  
}

void printGyro(){
  imu.readGyro();
  Serial.print("Gx: ");
  Serial.print(imu.calcGyro(imu.gx), 2);
  Serial.print(", Gy: ");
  Serial.print(imu.calcGyro(imu.gy), 2);
  Serial.print(", Gz: ");
  Serial.println(imu.calcGyro(imu.gz), 2);
}

void printAccel(){
  imu.readAccel();
  Serial.print("Ax: ");
  Serial.print(imu.calcAccel(imu.ax), 2);
  Serial.print(", Ay: ");
  Serial.print(imu.calcAccel(imu.ay), 2);
  Serial.print(", Az: ");
  Serial.println(imu.calcAccel(imu.az), 2);
}

void printMag(){
  imu.readMag();
  Serial.print("Mx: ");
  Serial.print(imu.calcMag(imu.mx), 2);
  Serial.print(", My: ");
  Serial.print(imu.calcMag(imu.my), 2);
  Serial.print(", Mz: ");
  Serial.println(imu.calcMag(imu.mz), 2);
}
