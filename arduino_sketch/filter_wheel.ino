# include <Servo.h>

// Define pin for servo motor
const int servoPin = 9;

// Create Servo object
Servo filterServo;

// Variable to store the position of the servo
int pos = 0;

void setup() {
  // Start serial communication
  Serial.begin(115200);
  // Attach the servo to the specified pin
  filterServo.attach(servoPin);

  // Move servo to home position
  home();
}

void loop() {
  // Check if there are any serial commands available
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
}

void processCommand(String command) {
  if (command == "ID") {
    Serial.println("Filter Wheel v1.0");
  }
  else if (command == "HOME") {
    home();
  }
  else if (command == "POS?") {
    Serial.println(pos);
  }
  else if (command.startsWith("GOTO")) {
    int newPos = command.substring(5).toInt(); // Get the position from the command
    gotoPosition(newPos);
  }
  else {
    Serial.println("Unknown command");
  }
}

void home() {
  pos = 0; // Set position to home
  filterServo.write(pos); // Move servo to home position
  Serial.println("Homed to position: " + String(pos));
  delay(1000);
}

void gotoPosition(int newPos) {
  if (newPos >= 0 && newPos <= 180) {
    pos = newPos; // Update position
    filterServo.write(pos); // Move servo to new position
    Serial.println("Moving to position: " + String(pos));
  } else {
    Serial.println("Position out of range");
  }
}