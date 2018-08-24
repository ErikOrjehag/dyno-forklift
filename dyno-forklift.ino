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
  demo();
}

void loop()
{
}

float cap(float value, float miin, float maax) {
  if (value < miin) return miin;
  else if (value > maax) return maax;
  else return value;
}

void homeLift() {
  liftDown(150);
  while (!readHomeSwitch());
  liftUp(100);
  delay(10); // Debounce noisy switch
  while (readHomeSwitch());
  liftStop();
}

void liftStop() {
  analogWrite(PIN_LIFT_DOWN, 0);
  analogWrite(PIN_LIFT_UP, 0);
}

void liftUp(int pwm) {
  analogWrite(PIN_LIFT_DOWN, 0);
  analogWrite(PIN_LIFT_UP, pwm);
}

void liftDown(int pwm) {
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

void setSteer(float steer) {
  steer = cap(steer, -1, 1);
  const int HALF = 4094 / 2;
  const int QUARTER = HALF / 2;
  const int STRIGHT = HALF - 65; // encoder has slight offset
  dxl_wb.goalPosition(DXL_STEER_ID, STRIGHT + QUARTER * steer);
}

void setDriveSpeed(float speed) {
  speed = cap(speed, -1, 1);
  const int MAX_SPEED = 300;
  dxl_wb.goalSpeed(DXL_DRIVE_ID, MAX_SPEED * speed * -1);
}

bool readHomeSwitch() {
  return !digitalRead(PIN_HOME_SWITCH);
}

void demo() {
  setSteer(0);
  delay(1000);
  setDriveSpeed(0.5);
  delay(2500);
  setDriveSpeed(0);
  delay(1000);
  setDriveSpeed(-0.25);
  delay(2500*2);
  setDriveSpeed(0.0);
  delay(1000);
  liftUp(200);
  delay(1000);
  liftStop();
  delay(1000);
  setSteer(1.0);
  setDriveSpeed(-1.0);
  delay(3000);
  setSteer(0.0);
  delay(1000);
  setSteer(-0.5);
  delay(2000);
  setSteer(0.0);
  setDriveSpeed(0.0);
  delay(1000);
  homeLift();
  setDriveSpeed(0.5);
  delay(3000);
  setDriveSpeed(0.0);
}



