#include "Wire.h"

#include "Adafruit_BMP280.h"
#include "Servo.h"

#define servo1Pin 3
#define servo2Pin 4

//#define atmosPressure 1013.25 //1023.25 hpa

#define dropAlt 120 //set drop altitude here
#define cutOffTime 2000 // drop reaching target alt for 2 second 

float groundAlt = 0.0;
long previousTime = 0;
bool drop = false;

Servo servo1;
Servo servo2;
Adafruit_BMP280 bmp;

void setup() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  servo1.write(0);
  servo2.write(0);

  Wire.begin();
  bmp.begin();

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  groundAlt = bmp.readAltitude();
}

void loop() {
//  previousTime = millis();

  if(bmp.readAltitude() - groundAlt < dropAlt){ //record the time when current alt is lower then dropAlt
    previousTime = millis();
  }

  if(millis() - previousTime > cutOffTime){ //if the time is not reset, drop 
    servo1.write(180);
    servo2.write(180);
  }



}
