#include <Arduino.h>
#include <MPU6050_light.h>
#include "Wire.h"

MPU6050 mpu(Wire);

const int leftV = 10;
const int leftC = 11;
const int leftD = 7;

const int rightV = 6;
const int rightC = 3;
const int rightD = 9;

// PID Values
float proportionalGain = 10.0f;
float integralGain = 0.0f;
float derivativeGain = 2.5f;

float previousError = 0.0f;
float integral = 0.0f;

float currentRotation = 0.0f;

void setup() {
    Serial.begin(9600);
    pinMode(leftV, OUTPUT);
    pinMode(leftC, OUTPUT);
    pinMode(leftD, OUTPUT);

    pinMode(rightV, OUTPUT);
    pinMode(rightC, OUTPUT);
    pinMode(rightD, OUTPUT);

    digitalWrite(leftV, HIGH);
    analogWrite(leftV, 90);
    digitalWrite(leftC, HIGH);
    digitalWrite(leftD, LOW);

    digitalWrite(rightV, HIGH);
    analogWrite(rightV, 90);
    digitalWrite(rightC, HIGH);
    digitalWrite(rightD, LOW);

    Serial.println("Initializing IMU");
    Wire.begin();
    mpu.begin();
    mpu.calcGyroOffsets();
    Serial.println("Setup complete");
}

void loop() {
    mpu.update();
    currentRotation += mpu.getGyroY();

    float error = -currentRotation;
    integral += error;
    previousError = error;

    // Generate the control value
    float proportional = error * proportionalGain;
    float integral = error * integralGain;
    float derivative = derivativeGain * (error - previousError);
    float motorControl = proportional + integral + derivative;
    bool invert = motorControl < 0;

    if (invert) {
        motorControl *= -1;
    }

    float pwmValue = mapControl(motorControl, 0, 2500, 100, 255);
    Serial.print(currentRotation);
    Serial.print("\n");

    digitalWrite(leftV, HIGH);
    digitalWrite(leftC, invert ? LOW : HIGH);
    digitalWrite(leftD, invert ? HIGH : LOW);
    analogWrite(leftV, pwmValue);

    digitalWrite(rightV, HIGH);
    digitalWrite(rightC, invert ? LOW : HIGH);
    digitalWrite(rightD, invert ? HIGH : LOW);
    analogWrite(rightV, pwmValue);
}

float mapControl(float value, float pidMin, float pidMax, float motorMin, float motorMax) {
    double pidRange = pidMax - pidMin;
    double motorRange = motorMax - motorMin;

    float mappedValue = (value - pidMin) * (motorRange / pidRange) + motorMin;

    if (mappedValue < motorMin) {
        mappedValue = motorMin;
    } else if (mappedValue > motorMax) {
        mappedValue = motorMax;
    }

    return mappedValue;
}