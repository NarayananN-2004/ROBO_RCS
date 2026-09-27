// R2-D2 bot - Uno + L293D motor shield, HC-05 on D0/D1 (hardware serial)
// M1 = Left, M2 = Right, M3 = Front (M4 unused)
// Servo on SER1 header (D10). LEDs on A1 (green/safe), A2 (red/danger).
//
// IMPORTANT: unplug the HC-05 (or its D0/D1 wires) before uploading,
// since it shares those pins with the USB programming port.
//
// Bluetooth commands:
// F B L R S -> drive / stop
// Q E C     -> head left / right / center
// G         -> green (safe) eye
// D         -> red (danger) eye
// O         -> both LEDs off

#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motor1(1, MOTOR12_1KHZ); // Left
AF_DCMotor motor2(2, MOTOR12_1KHZ); // Right
AF_DCMotor motor3(3, MOTOR34_1KHZ); // Front

Servo myservo;

#define LED_G A1
#define LED_R A2

#define HEAD_MIN    30
#define HEAD_MAX    150
#define HEAD_CENTER 90

int driveSpeed = 200;
int turnSpeed  = 170;
int headPos    = HEAD_CENTER;
char key;

void setup() {
  Serial.begin(9600);       // HC-05 on D0/D1, hardware serial

  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  setEye(true, false);      // default: safe/green

  myservo.attach(10);
  myservo.write(headPos);

  Stop();
}

void loop() {
  remotecontrol();
}

void remotecontrol() {
  if (Serial.available() > 0) {
    key = toupper(Serial.read());
    switch (key) {
      case 'F': forward();  break;
      case 'B': backward(); break;
      case 'L': left();     break;
      case 'R': right();    break;
      case 'S': Stop();     break;
      case 'Q':
        headPos = constrain(headPos - 15, HEAD_MIN, HEAD_MAX);
        myservo.write(headPos);
        break;
      case 'E':
        headPos = constrain(headPos + 15, HEAD_MIN, HEAD_MAX);
        myservo.write(headPos);
        break;
      case 'C':
        headPos = HEAD_CENTER;
        myservo.write(headPos);
        break;
      case 'G': setEye(true, false);  break;
      case 'D': setEye(false, true);  break;
      case 'O': setEye(false, false); break;
    }
  }
}

void forward() {
  motor1.setSpeed(driveSpeed);
  motor2.setSpeed(driveSpeed);
  motor3.setSpeed(driveSpeed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
}

void backward() {
  motor1.setSpeed(driveSpeed);
  motor2.setSpeed(driveSpeed);
  motor3.setSpeed(driveSpeed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
}

void left() {
  motor1.setSpeed(turnSpeed);
  motor2.setSpeed(turnSpeed);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(RELEASE);   // front wheel free-rolls during a turn
}

void right() {
  motor1.setSpeed(turnSpeed);
  motor2.setSpeed(turnSpeed);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(RELEASE);
}

void Stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
}

void setEye(bool green, bool red) {
  digitalWrite(LED_G, green);
  digitalWrite(LED_R, red);
}
