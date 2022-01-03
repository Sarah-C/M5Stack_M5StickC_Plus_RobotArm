#ifndef ROBOTARMMOVER_H
#define ROBOTARMMOVER_H

#include <M5StickCPlus.h>
#include "CBACK_DRIVER.h"
#include "trajectory.h"

#define UPDATE_FREQUENCY 150
#define UPDATE_TIME (1000 / UPDATE_FREQUENCY)

class RobotArmMover {
  private:
  void nextParkStage();
  
  public:
    RobotArmMover();
    ~RobotArmMover();
    int8_t parkStage = 0;
    uint8_t speed = 0;
    int8_t calibration[4] = {0, 0, 0, 25};
    unsigned long updateTimer;
    CBACK_DRIVER Driver;
    void init();
    void park();
    bool ready();
    void update();
    void setSpeed(uint8_t newSpeed);
    void setBase(uint8_t angle);
    void setLowerArm(uint8_t angle);
    void setUpperArm(uint8_t angle);
    void setGripper(uint8_t amount);
    float getBase();
    float getArm();
    float getForearm();
    float getGripper();
    
    //Trajectory servoTrajectory[];
};

extern Trajectory servoTrajectory[];

#endif
