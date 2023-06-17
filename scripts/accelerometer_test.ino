#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

void setup() {
    Serial.begin(9600);
    Serial.println("Initializing");
    Wire.begin();
    Serial.println("Initializing wire");
    mpu.begin();
    Serial.println("Starting MPU");
    Serial.println(F("Calculating gyro offset, do not move MPU6050")); delay(1000);
    mpu.calcGyroOffsets();
}

void loop() {
    mpu.update();
    Serial.print(F("TEMPERATURE : "));Serial.println(mpu.getTemp()); Serial.print(F("ACCELERO X : "));Serial.print(mpu.getAccX());
    Serial.print("\tY : ");Serial.print(mpu.getAccY());
    Serial.print("\tZ : ");Serial.println(mpu.getAccZ());
    Serial.print(F("GYRO X : "));
    Serial.print(mpu.getGyroX());
    Serial.print("\tY : ");
    Serial.print(mpu.getGyroY());
    Serial.print("\tZ : ");
    Serial.println(mpu.getGyroZ());
    Serial.println(F("=======================================================\n"));
    delay(2000);
}
