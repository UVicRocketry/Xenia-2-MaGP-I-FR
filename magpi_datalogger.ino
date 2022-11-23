//right now must clear sd card every time, need to make more robust
//also record time

#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include "Adafruit_BMP3XX.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define BMP_SCK 

const int chipSelect = BUILTIN_SDCARD;
const int collectionRate = 500;

Adafruit_BMP3XX bmp;
Adafruit_MPU6050 mpu;

void setup() {
  //for testing
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  Wire2.begin();
  Wire2.setSCL(24);
  Wire2.setSDA(25);

  //open sd card and write header
  String header = "*C, Pa, Alt (m), x (m/s^2), y (m/s^2), z (m/s^2), x (deg/s), y (deg/s), z (deg/s),";
  if (SD.begin(chipSelect)) {
    File dataFile = SD.open("data.txt", FILE_WRITE);
    dataFile.println(header);
    dataFile.close();
  }

  // Set up bmp 390
  
  if (!bmp.begin_I2C(0x77 ,&Wire2)) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  // Set up mpu

   if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while(1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  

  
}

void loop() {
  // put your main code here, to run repeatedly:

  // make a string for assembling the data to log and a temporary variable x to store data before append to string:
  String dataString = "";
  float x;
  
  //read and append string

  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  
  x = bmp.temperature;
  dataString += String(x);
  dataString+= ", ";

  x = bmp.pressure;
  dataString += String(x);
  dataString+= ", ";

  x = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  dataString += String(x);
  dataString+= ", ";

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  x = a.acceleration.x;
  dataString += String(x);
  dataString+= ", ";

  x = a.acceleration.x;
  dataString += String(x);
  dataString+= ", ";

  x = a.acceleration.y;
  dataString += String(x);
  dataString+= ", ";

  x = a.acceleration.z;
  dataString += String(x);
  dataString+= ", ";

  x = g.gyro.x;
  dataString += String(x);
  dataString+= ", ";

  x = g.gyro.y;
  dataString += String(x);
  dataString+= ", ";

  x = g.gyro.z;
  dataString += String(x);
  dataString+= ", ";
  
  Serial.println(dataString);
  
  //store
  File dataFile = SD.open("data.txt", FILE_WRITE);
  dataFile.println(dataString);
  dataFile.close();
  delay(collectionRate); 

}
