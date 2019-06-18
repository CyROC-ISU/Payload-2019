// made by cream puff daddy

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno1 = Adafruit_BNO055(55, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(55, 0x29);
#define LED_BUILTIN 13
File logfileStatic;

void signSD(File *file, double d);

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    while (1) {
      Serial.println("SD initialization failed!");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }

  char filename[15];
  strcpy(filename, "STATIC00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i / 10;
    filename[7] = '0' + i % 10;
    if (!SD.exists(filename)) {
      break;
    }
  }
  logfileStatic = SD.open(filename, FILE_WRITE);
  strcpy(filename, "DYNAMIC00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = '0' + i / 10;
    filename[8] = '0' + i % 10;
    if (!SD.exists(filename)) {
      break;
    }
  }
  logfileStatic.println("Starting static BNO055 test");

  /* Initialise the sensor */
  if (!bno1.begin()) {
    while (1) {
      Serial.println("BNO1 initialization failed!");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }
  bno1.setExtCrystalUse(true);
  if (!bno2.begin()) {
    while (1) {
      Serial.println("BNO2 initialization failed!");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }
  bno2.setExtCrystalUse(true);
  Serial.print("Starting test");
  logfileStatic.println("Starting Payload test SAC 2019");
}

void loop(void) {
  imu::Vector<3> acc1 = bno1.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> acc2 = bno2.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  logfileStatic.print("BNO1:,");
  signSD(&logfileStatic, acc1[0]);
  logfileStatic.print(",");
  signSD(&logfileStatic, acc1[1]);
  logfileStatic.print(",");
  signSD(&logfileStatic, acc1[2]);
  logfileStatic.print(",,,");

  logfileStatic.print("BNO2:,");
  signSD(&logfileStatic, acc2[0]);
  logfileStatic.print(",");
  signSD(&logfileStatic, acc2[1]);
  logfileStatic.print(",");
  signSD(&logfileStatic, acc2[2]);
  logfileStatic.println();
  logfileStatic.flush();
  delay(50);
}

void signSD(File *f, double d) {
  if (d >= 0.0) {
    f->print(" ");
  }
  f->print(d, 3);
}
