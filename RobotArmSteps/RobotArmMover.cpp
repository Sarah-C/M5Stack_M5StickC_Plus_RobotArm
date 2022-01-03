#include "RobotArmMover.h"

/*

  Base :
  0=Far right, 180=Far left

  Arm :
  0=Straight up, 80=Straight ahead, 95=down

  Forearm :
  0=Straight up, 75=Straight ahead,

  Clamp :
  (0 to 90) 35=Closed, 130=Open

*/

// Dynamics(max velocity >F, acceleration >S, deceleration >S, threshold)
Trajectory servoTrajectory[] = {
  Trajectory(140, 100, 80, 0.05),
  Trajectory(140, 100, 80, 0.05),
  Trajectory(140, 100, 80, 0.05),
  Trajectory(140, 100, 80, 0.05)
};

RobotArmMover::RobotArmMover() {
  Driver.Init();
}

RobotArmMover::~RobotArmMover() {
}

void RobotArmMover::init() {
  Driver.SetServoAngle(1, 90 + calibration[0]);
  Driver.SetServoAngle(2, 10 + calibration[1]);
  Driver.SetServoAngle(3, 90 + calibration[2]);
  Driver.SetServoAngle(4, 35 + calibration[3]);
  servoTrajectory[0].reset(90);
  servoTrajectory[1].reset(10);
  servoTrajectory[2].reset(90);
  servoTrajectory[3].reset(35);
  updateTimer = millis();
}

void RobotArmMover::park() {
  if (parkStage) return;
  parkStage = 1;
}

void RobotArmMover::nextParkStage() {
  switch (parkStage) {
    case 1:
      //servoTrajectory[0].setTargetPos(90, 5);
      servoTrajectory[1].setTargetPos(5, 3);
      servoTrajectory[2].setTargetPos(70, 3);
      //servoTrajectory[3].setTargetPos(35, 5);
      break;

    case 2:
      servoTrajectory[0].setTargetPos(90, 3);
      //servoTrajectory[1].setTargetPos(10, 5);
      //servoTrajectory[2].setTargetPos(90, 5);
      //servoTrajectory[3].setTargetPos(35, 5);
      break;

    case 3:
      //servoTrajectory[0].setTargetPos(90, 5);
      //servoTrajectory[1].setTargetPos(10, 5);
      //servoTrajectory[2].setTargetPos(90, 5);
      servoTrajectory[3].setTargetPos(20, 3);
      break;

    default:
      // Parked
      parkStage = -1;
  }
  parkStage++;
}

bool RobotArmMover::ready() {
  return servoTrajectory[0].ready() && servoTrajectory[1].ready() && servoTrajectory[2].ready() && servoTrajectory[3].ready();
}

void RobotArmMover::update() {
  if (millis() - updateTimer >= UPDATE_TIME) {
    updateTimer += UPDATE_TIME;
    Driver.SetServoAngle(1, round(servoTrajectory[0].update()) + calibration[0]);
    Driver.SetServoAngle(2, round(servoTrajectory[1].update()) + calibration[1]);
    Driver.SetServoAngle(3, round(servoTrajectory[2].update()) + calibration[2]);
    Driver.SetServoAngle(4, round(servoTrajectory[3].update()) + calibration[3]);
    if (parkStage > 0) {
      if (ready()) nextParkStage();
    }
  }
}

void RobotArmMover::setBase(uint8_t angle) {
  if (parkStage) return;
  if (speed == 0)
    servoTrajectory[0].setTargetPos(angle);
  else
    servoTrajectory[0].setTargetPos(angle, speed);
}

void RobotArmMover::setLowerArm(uint8_t angle) {
  if (parkStage) return;
  if (speed == 0)
    servoTrajectory[1].setTargetPos(angle);
  else
    servoTrajectory[1].setTargetPos(angle, speed);
}

void RobotArmMover::setUpperArm(uint8_t angle) {
  if (parkStage) return;
  if (speed == 0)
    servoTrajectory[2].setTargetPos(angle);
  else
    servoTrajectory[2].setTargetPos(angle, speed);
}

void RobotArmMover::setGripper(uint8_t amount) {
  if (parkStage) return;
  if (amount > 100) amount = 100;
  servoTrajectory[3].setTargetPos(amount);
}

float RobotArmMover::getBase() {
  return servoTrajectory[0].getPos();
}

float RobotArmMover::getArm() {
  return servoTrajectory[1].getPos();
}

float RobotArmMover::getForearm() {
  return servoTrajectory[2].getPos();
}

float RobotArmMover::getGripper() {
  return servoTrajectory[3].getPos();
}

void RobotArmMover::setSpeed(uint8_t newSpeed) {
  speed = newSpeed;
}
