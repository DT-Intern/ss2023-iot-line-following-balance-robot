#include <Arduino.h>
#include "MPU6050.h"
#include "Wire.h"

MPU6050 gyro;
int16_t gx, gy, gz;

const int leftV = 10;
const int rightV = 6;

void setup() {
    Serial.begin(38400);

    pinMode(leftV, OUTPUT);
    pinMode(rightV, OUTPUT);

    digitalWrite(leftV, LOW);
    digitalWrite(rightV, LOW);

    Serial.println("Initializing IMU");
    Wire.begin();
    gyro.initialize();
    Serial.println(gyro.testConnection() ? "Connected" : "Failed");
    Serial.println("Setup complete");
}

void loop() {
    gyro.getRotation(&gx, &gy, &gz);

    Serial.print(gx);
    Serial.print("\t");
    Serial.print(gy);
    Serial.print("\t");
    Serial.print(gz);
    Serial.print("\n");
    delay(1000);
}