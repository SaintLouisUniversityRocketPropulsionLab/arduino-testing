//Serial output of LSM9DS1 and BME280 CSV
#include <Wire.h>
#include <SparkFunLSM9DS1.h>
#include <SparkFunBME280.h>
#include <SPI.h>
#include <SD.h>

LSM9DS1 imu;
BME280 bme;
const int chipSelect = 10;
char fileName[16];
unsigned int fnum = 1;
String dataString = "";
int good = 8;
int bad = 6;

//I2C setup
#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B

//Output
#define PRINT_CALCULATED
#define PRINT_SPEED 250
static unsigned long lastPrint = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(good, OUTPUT);
  pinMode(bad, OUTPUT);
  digitalWrite(good, HIGH);
  digitalWrite(bad, HIGH);
  delay(1000);
  digitalWrite(bad, LOW);

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  Wire.begin();
  
  if (!SD.begin(chipSelect)){
    while (1);
  }

  while (!fnum==0){
    sprintf(fileName, "file_%03d.csv", fnum);
    if (SD.exists(fileName)==false){
      break;
    }
    fnum++;
  }
  File dataFile = SD.open(fileName, FILE_WRITE);

  if (!imu.begin()){
    dataFile.println("IMU did not respond");
    digitalWrite(bad, HIGH);
    delay (1000);
    digitalWrite(bad, LOW);
  }
  if (!bme.beginI2C()){
    dataFile.println("BME did not respond.");
    digitalWrite(bad, HIGH);
    delay (1000);
    digitalWrite(bad, LOW);
  }
  dataFile.close();  
  printHeader();
}

void loop() {
  File dataFile = SD.open(fileName, FILE_WRITE);
  dataString = String(micros());

  recordTemp();
  recordPress();
  recordAccel();
  recordGyro();
  recordMag();
  
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    digitalWrite(good, HIGH);
  }
  else{
    digitalWrite(bad, HIGH);
    digitalWrite(good, LOW);
  }  
}

void recordTemp(){
  dataString += ",";
  dataString += String(bme.readTempC(), 2);
}

void recordPress(){
  dataString += ",";
  dataString += String(bme.readFloatPressure(), 1);
}

void recordGyro(){
  imu.readGyro();
  dataString += ",";
  dataString += String(imu.calcGyro(imu.gx), 2);
  dataString += ",";
  dataString += String(imu.calcGyro(imu.gy), 2);
  dataString += ",";
  dataString += String(imu.calcGyro(imu.gz), 2);
}

void recordAccel(){
  imu.readAccel();
  dataString += ",";
  dataString += String(imu.calcAccel(imu.ax), 2);
  dataString += ",";
  dataString +=  String(imu.calcAccel(imu.ay), 2);
  dataString += ",";
  dataString += String(imu.calcAccel(imu.az), 2);
}

void recordMag(){
  imu.readMag();
  dataString += ",";
  dataString += String(imu.calcMag(imu.mx), 2);
  dataString += ",";
  dataString += String(imu.calcMag(imu.my), 2);
  dataString += ",";
  dataString += String(imu.calcMag(imu.mz), 2);
}

void printHeader(){
  File dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.println("Time,Temp,Press,Gx,Gy,Gz,Ax,Ay,Az,Mx,My,Mz");
    dataFile.close();
  }
  else{
    digitalWrite(bad, HIGH);
  }
}
