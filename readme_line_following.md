# IoT Line-Following-Robot

## Group Members

- Maximilian Flügel (70481185): Program Logic
- Alina Simon (70481013): Casing/Electronics
- Jannes Bikker (MatrikelNr): Program Logic

## Description

This project is a group / classroom project for the lecture: Systems and Control Engineering - Technik und IoT. The purpose of the project is to demonstrate the use of IoT devices in a practical application. 
This particular project features line sensors and an infrared sensor as well as two motors connected trough a push-pull four channel driver. The aim of this project is a robot which can follow a line at the ground and stops, if there is a obstacle in front of it.
Doing so, the robot should not cross the line and should be able to drive again if the obstacle is out of the way.

## Parts and Schematics

### Assembly

|NAME|QUANTITY|COMPONENT|
|-|-|-|
|U1|1|Arduino Uno R3|
|U2|1|IR-Sensor|
|U3|1|Push-pull four channel driver|
|M1,M2|2|Motor|
|D1-D8|8|Diode|
|PIR1, PIR2|2|IR-reflector-Sensor|

![Assembly](/circuits/line_following/LineFollowingRobotCircuit.png)
![Circuit](/circuits/line_following/LineFollowingRobotCircuit.pdf)

## Structure of the program

### Startup

To start the robot, make sure to place the robot, so the line on the ground is between its wheels.

### Main Program

Now the robot will follow the line and stop befre upcoming obstacles.

**Measurement Process**


The used infrared sensor delivers voltage. To calculate the distance to the measured object, we used the following formula:

$distance = (3027.4 / voltage)^(1.2134)$

We decided to stop the robot at 13cm distance to objects.
If there is no object detected, we measure the brightness of the ground with two IR-reflector-Sensors to acknowledge if the robot have to steer.
A darker ground implicates higher sensor data values. As the gound is black and the line we want to follow is white, lower values at the left sided sensor means the robot have to steer to the right side. In this case we turn off the motor steering the right wheel. Otherwise the right motor will turn.
Analog to the leftsided sensor the rightsided sensor influences the left motor.

## Problems and Difficulties

### Stray Light

One problem of the ground measuring process was that the ir-reflector-sensor get influences a lot by light from the sides instead of only the reflection from the ground. To eliminate this problem, we assembled the ir-reflector-sensors with a screw directly at the buttom of the robot to minimize the distance between ground and sensor.


### Assembly

Our first idea was to build a car on four wheels.