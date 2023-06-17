#include <Arduino.h>

const int leftV = 10;
const int leftC = 11;
const int leftD = 7;

const int rightV = 6;
const int rightC = 3;
const int rightD = 9;

void setup() {
    pinMode(leftV, OUTPUT);
    pinMode(leftC, OUTPUT);
    pinMode(leftD, OUTPUT);

    pinMode(rightV, OUTPUT);
    pinMode(rightC, OUTPUT);
    pinMode(rightD, OUTPUT);

    digitalWrite(leftV, HIGH);
    digitalWrite(leftC, HIGH);
    digitalWrite(leftD, LOW);

    digitalWrite(rightV, HIGH);
    digitalWrite(rightC, HIGH);
    digitalWrite(rightD, LOW);

    delay(1000);

    digitalWrite(leftV, HIGH);
    digitalWrite(leftC, LOW);
    digitalWrite(leftD, HIGH);
    digitalWrite(rightV, HIGH);
    digitalWrite(rightC, LOW);
    digitalWrite(rightD, HIGH);

    delay(1000);

    digitalWrite(leftV, LOW);
    digitalWrite(rightV, LOW);
}

void loop() {

}
