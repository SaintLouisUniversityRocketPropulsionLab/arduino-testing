//Serial output of LSM9DS1 and BME280 CSV
#include <Wire.h>
#include <SparkFunLSM9DS1.h>
#include <SparkFunBME280.h>
#include <SPI.h>
#include <SD.h>

LSM9DS1 imu;
BME280 bme;
const int chipSelect = 10;

//I2C setup
#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B

//Output
#define PRINT_CALCULATED
#define PRINT_SPEED 250
static unsigned long lastPrint = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  Wire.begin();
  
  if (!imu.begin()){
    Serial.println("IMU did not respond.");
    delay (1000);
  }
  if (!bme.beginI2C()){
    Serial.println("BME did not respond.");
    delay (1000);
  }
  if (!SD.begin(chipSelect)){
    Serial.println("SD card did not respond.");
    while (1);
  }
  Serial.println("SD card initialized");
  printHeader();
}

void loop() {
  Serial.print(micros());
  printTemp();
  printPress();
  printGyro();
  printAccel();
  printMag();
  
}

void printTemp(){
  Serial.print(", ");
  Serial.print(bme.readTempC(), 2);
}

void printPress(){
  Serial.print(", ");
  Serial.print(bme.readFloatPressure(), 1);
}

void printGyro(){
  imu.readGyro();
  Serial.print(", ");
  Serial.print(imu.calcGyro(imu.gx), 2);
  Serial.print(", ");
  Serial.print(imu.calcGyro(imu.gy), 2);
  Serial.print(", ");
  Serial.print(imu.calcGyro(imu.gz), 2);
}

void printAccel(){
  imu.readAccel();
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.ax), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.ay), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.az), 2);
}

void printMag(){
  imu.readMag();
  Serial.print(", ");
  Serial.print(imu.calcMag(imu.mx), 2);
  Serial.print(", ");
  Serial.print(imu.calcMag(imu.my), 2);
  Serial.print(", ");
  Serial.println(imu.calcMag(imu.mz), 2);
}

void printHeader(){
  Serial.println("Time, Temp, Press, Gx, Gy, Gz, Ax, Ay, Az, Mx, My, Mz");
}
