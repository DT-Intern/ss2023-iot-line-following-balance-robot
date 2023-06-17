#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"

#define SSID "iPhone$von$Max"
#define KEY "test1234"

int analogValue;
int currentDistance;

const int inv = 7;
const int Vinh = 10;
int motorSpeedA = 0;

const int inv2 = 3;
const int Vinh2 = 9;
int motorSpeedB = 0;

const int LEFT_LINE_SENSOR = 1;
int leftLineSensorValue;
const int RIGHT_LINE_SENSOR = 2;
int rightLineSensorValue;

SoftwareSerial uart(2, 3);
WiFly wifly(&uart);

/**
 * Setup method of the program.
 * This method configures all input and output pins and starts the UART and Serial interfaces.
 * Furthermore, this method invokes the methods for configuring WiFly and Network capabilities.
 */
void setup() {
    //pinMode(A0, INPUT);
    pinMode(inv, OUTPUT);
    pinMode(Vinh, OUTPUT);
    pinMode(inv2, OUTPUT);
    pinMode(Vinh2, OUTPUT);

    uart.begin(9600);
    Serial.begin(9600);
    //delay(1000);

    //configureWifly();
    //connectAndVerifyNetwork();

    motorSpeedA = 255;
    motorSpeedB = 255;

    digitalWrite(inv, HIGH);
    analogWrite(Vinh, motorSpeedA);
    digitalWrite(inv2, HIGH);
    analogWrite(Vinh2, motorSpeedB);
}

/**
 * Method used for WiFly configuration.
 * This method starts by resetting the WiFi-Interface.
 * After that, the local port is set to 80 (HTTP default).
 * Furthermore, the default string is set to "0" (none) and new requests are buffered to *OPEN*.
 */
void configureWifly() {
    wifly.reset();
    Serial.println("Starting");

    // Update the preferences
    wifly.sendCommand("set ip local 80\r");
    wifly.sendCommand("set comm remote 0\r");
    wifly.sendCommand("set comm open *OPEN*\r");
}

/**
 * Method that establishes the connection to the WiFi network and verifies the connection.
 * The SSID and KEY variables are used for the connection.
 * By default, WPA2 PSK (Pre-Shared-Key) is used for network authentication.
 * After the connection has been established, the controller waits 5 seconds for IP-Address association.
 * After the 5 seconds, the command to retrieve the current IP is invoked and then printed to the Serial monitor.
 */
void connectAndVerifyNetwork() {
    // Connect to the configured network
    while (wifly.join(SSID, KEY, WIFLY_AUTH_WPA2_PSK) == false) {
        Serial.println("Unable to connect");
    }

    Serial.println("Connected");

    //Attempt to fetch the IP-Address
    delay(5000);
    wifly.sendCommand("get ip\r");

    // Print the result
    unsigned char c;
    while (wifly.receive((uint8_t *)&c, 1, 300) > 0) {
        Serial.print((char)c);
    }
}

void loop() {
    // Fetch the latest sensor values
    // updateSensorValues();

    // Serve the web interface
    // serveWebInterface();
}

void updateSensorValues() {
    // Infrared
    analogValue = analogRead(A0);
    currentDistance = pow((3027.4 / analogValue), 1.2134);

    // Line sensor
    leftLineSensorValue = analogRead(LEFT_LINE_SENSOR);
    rightLineSensorValue = analogRead(RIGHT_LINE_SENSOR);
}

void pollValues() {
    String result = "";
    result += "{\n";
    result += "\t\"distance\": ";
    result += currentDistance;
    result += ",\n";
    result += "\t\"leftLineSensor\": ";
    result += leftLineSensorValue;
    result += ",\n";
    result += "\t\"rightLineSensor\": ";
    result += rightLineSensorValue;
    result += "\n";
    result += "}";

    uart.print(F("\nContent-Length: "));
    uart.print(result.length());
    uart.print("\n");
    uart.println();
    uart.print(result);
}

void serveWebInterface() {
    if (wifly.available()) {
        if (uart.find("*OPEN*")) {
            delay(1000);
            if (uart.find("action=")) {
                Serial.println("Action detected");
                int action = uart.read() - 48;
                Serial.println(action);

                if (action == 0) {
                    Serial.println("Polling values");
                    uart.print(F("HTTP/1.1 200 OK\nContent-Type: application/json\nConnection: close"));
                    pollValues();
                }
            } else {
                // Default destination: dashboard
                Serial.println(F("Requested dashboard"));
                uart.print(F("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 2064\nConnection: close\n\n"));
                uart.print(F("<!DOCTYPE html><html><head><title>IoT Line Following Robot Dashboard</title><link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/semantic-ui/2.5.0/semantic.min.css\" integrity=\"sha512-KXol4x3sVoO+8ZsWPFI/r5KBVB/ssCGB5tsv2nVOKwLg33wTFP3fmnXa47FdSVIshVTgsYk/1734xSk9aFIa4A==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\" /></head><body><div class=\"ui container\" style=\"margin: 1rem\"><h1>IoT Line Following Robot Dashboard</h1><h3 style=\"margin-top: 0; margin-bottom: 2.5rem\">By Alina Simon, Jannes Bikker and Maximilian Fluegel</h3><h4 id=\"infraredValueOutput\">Current Infrared Value: 0</h4><h4 id=\"primaryLineSensorOutput\">Current Primary Line Sensor Value: 0</h4><h4 id=\"secondaryLineSensorOutput\">Current Secondary Line Sensor Value: 1</h4><h4 id=\"tertiaryLineSensorOutput\">Current Tertiary Line Sensor Value 2</h4></div><script>const infraredValueOutput = document.getElementById(\"infraredValueOutput\");const primaryLineSensorOutput = document.getElementById(\"primaryLineSensorOutput\");const secondaryLineSensorOutput = document.getElementById(\"secondaryLineSensorOutput\");const tertiaryLineSensorOutput = document.getElementById(\"tertiaryLineSensorOutput\");const poll = () => {setTimeout(() => {fetch(\"/action=0\").then(raw => raw.json()).then(data => {infraredValueOutput.innerHTML = `Current Infrared Value: ${data.distance}`;primaryLineSensorOutput.innerHTML = `Current Primary Line Sensor Value: ${data.primaryLineSensor}`;secondaryLineSensorOutput.innerHTML = `Current Secondary Line Sensor Value: ${data.secondaryLineSensor}`;tertiaryLineSensorOutput.innerHTML = `Current Tertiary Line Sensor Value: ${data.tertiaryLineSensor}`;}).catch(() => {infraredValueOutput.innerHTML = `Current Infrared Value: Unable to poll`;primaryLineSensorOutput.innerHTML = `Current Primary Line Sensor Value: Unable to poll`;secondaryLineSensorOutput.innerHTML = `Current Secondary Line Sensor Value: Unable to poll`;tertiaryLineSensorOutput.innerHTML = `Current Tertiary Line Sensor Value: Unable to poll`;});poll();}, 2000);};poll();</script></body></html>"));
            }
        }
    }
}