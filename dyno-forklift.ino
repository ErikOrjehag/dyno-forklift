#include <DynamixelWorkbench.h>

#define DEVICE_NAME "" //Dynamixel on Serial3(USART3)  <-OpenCR
const int BAUDRATE = 1000000;
const int DXL_STEER_ID = 1;
const int DXL_DRIVE_ID = 2;

const int PIN_LIFT_UP = 10;
const int PIN_LIFT_DOWN = 9;
const int PIN_HOME_SWITCH = 4;

DynamixelWorkbench dxl_wb;

bool readHomeSwitch();

void setup()
{
  Serial.begin(9600);
  setupDriveWheel();
  setupLift();
  homeLift();
}

void loop()
{
  //driveWheelDemo();
  //Serial.println("home: " + String(readHomeSwitch()));
}

void homeLift() {
  liftDown(150);
  while (!readHomeSwitch());
  liftUp(100);
  delay(10);
  while (readHomeSwitch());
  liftStop();
}

void liftStop() {
  Serial.println("stop");
  analogWrite(PIN_LIFT_DOWN, 0);
  analogWrite(PIN_LIFT_UP, 0);
}

void liftUp(int pwm) {
  Serial.println("up");
  analogWrite(PIN_LIFT_DOWN, 0);
  analogWrite(PIN_LIFT_UP, pwm);
}

void liftDown(int pwm) {
  Serial.println("down");
  analogWrite(PIN_LIFT_UP, 0);
  analogWrite(PIN_LIFT_DOWN, pwm);
}

void setupLift() {
  pinMode(PIN_LIFT_UP, OUTPUT);
  pinMode(PIN_LIFT_DOWN, OUTPUT);
  pinMode(PIN_HOME_SWITCH, INPUT_PULLUP);
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
}

void setupDriveWheel() {
  dxl_wb.begin(DEVICE_NAME, BAUDRATE);
  dxl_wb.ping(DXL_STEER_ID);
  dxl_wb.ping(DXL_DRIVE_ID);
  dxl_wb.jointMode(DXL_STEER_ID);
  dxl_wb.wheelMode(DXL_DRIVE_ID);
}

void driveWheelDemo() {
  int full = 4094;
  int middle = full / 2;
  
  dxl_wb.goalPosition(DXL_STEER_ID, middle - middle/3);
  dxl_wb.goalSpeed(DXL_DRIVE_ID, 300);

  delay(1000);

  dxl_wb.goalPosition(DXL_STEER_ID, middle + middle/3);
  dxl_wb.goalSpeed(DXL_DRIVE_ID, -300);

  delay(1000);
}

bool readHomeSwitch() {
  return !digitalRead(PIN_HOME_SWITCH);
}

