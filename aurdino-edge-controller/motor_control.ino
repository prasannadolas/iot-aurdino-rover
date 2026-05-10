The Arduino Code (Motor control logic)

Connections:-
1. Motors to L298N Driver
Left Side: Connect the Front-Left and Back-Left motor wires together.
Connect both Red wires to OUT1.
Connect both Black wires to OUT2.
Right Side: Connect the Front-Right and Back-Right motor wires together.
Connect both Red wires to OUT3.
Connect both Black wires to OUT4.

2. L298N to Arduino Uno
ENA (Left Speed): Connect to Pin 5 (Remove the jumper cap if present).
IN1 (Left Direction): Connect to Pin 8.
IN2 (Left Direction): Connect to Pin 9.
IN3 (Right Direction): Connect to Pin 10.
IN4 (Right Direction): Connect to Pin 11.
ENB (Right Speed): Connect to Pin 6 (Remove the jumper cap).

3. Power (Crucial!)
12V Battery (+): Connect to L298N 12V screw terminal.
Battery (-): Connect to L298N GND.
Arduino GND: Connect to L298N GND (You MUST connect the Arduino Ground to the Battery Ground, or it won't work).
L298N 5V: Connect to Arduino Vin (This powers the Arduino from the battery).

4. NodeMCU to Arduino (The Communication Link)
NodeMCU TX: Connect to Arduino Pin 2.
NodeMCU RX: Connect to Arduino Pin 3.
GND: Connect NodeMCU GND to Arduino GND.

CODE:-
#include <SoftwareSerial.h>

// Define pins for communication with NodeMCU
// RX on Arduino (Connect to NodeMCU TX), TX on Arduino (Connect to NodeMCU RX)
SoftwareSerial nodeMcuSerial(2, 3); 

// Motor A Connections (Left Side Wheels)
int enA = 5;
int in1 = 8;
int in2 = 9;

// Motor B Connections (Right Side Wheels)
int enB = 6;
int in3 = 10;
int in4 = 11;

// Variable to store command
String command = "";
// Speed variable (0-255)
int motorSpeed = 150; // Start at moderate speed

void setup() {
  // Start Serial Monitor for debugging
  Serial.begin(9600);
  // Start communication with NodeMCU
  nodeMcuSerial.begin(9600);

  // Set all motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Initial state: Motors off
  stopCar();
  Serial.println("Arduino Ready. Waiting for commands...");
}

void loop() {
  // Check if data is coming from NodeMCU
  if (nodeMcuSerial.available() > 0) {
    // Read the incoming string until a newline character
    command = nodeMcuSerial.readStringUntil('\n');
    command.trim(); // Remove any extra spaces

    Serial.print("Command Received: ");
    Serial.println(command);

    // Execute function based on command
    if (command == "FORWARD") {
      moveForward();
    } 
    else if (command == "BACKWARD") {
      moveBackward();
    } 
    else if (command == "LEFT") {
      turnLeft();
    } 
    else if (command == "RIGHT") {
      turnRight();
    } 
    else if (command == "STOP") {
      stopCar();
    }
    // Optional: Add logic to change speed here if needed
  }
}

// --- Movement Functions ---

void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, motorSpeed);
  analogWrite(enB, motorSpeed);
}

void moveBackward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, motorSpeed);
  analogWrite(enB, motorSpeed);
}

void turnLeft() {
  // Left motors backward, Right motors forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, motorSpeed);
  analogWrite(enB, motorSpeed);
}

void turnRight() {
  // Left motors forward, Right motors backward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, motorSpeed);
  analogWrite(enB, motorSpeed);
}

void stopCar() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}