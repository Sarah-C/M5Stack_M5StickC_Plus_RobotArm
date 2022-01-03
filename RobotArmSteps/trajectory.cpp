#include "trajectory.h"

/**
 * Default constructor
 *
 * @param (_maxVel) Maximum velocity - default = 100
 * @param (_acc) Constant acceleration - default = 50
 * @param (_dec) Constant deceleration - default = same as acceleration
 * @param (_thesh) Cutoff threshold - default = 0.1
 */
Trajectory::Trajectory(float _maxVel, float _acc, float _dec, float _thresh) {
  type = 0;
  target = 0;
  curPos = 0;
  curVel = 0;
  maxVel = _maxVel;
  velGoal = _maxVel;
  acc = _acc;
  if (_dec == -1) dec = _acc;
  else dec = _dec;
  oldTime = millis();
  threshold = _thresh;
  noTasks = true;
}


/**
 * Default destructor
 */
Trajectory::~Trajectory() {
  // Empty
}


/**
 * Set a new target position
 *
 * @param  (_targetPos) New target position value
 * @note   This overides any previous position or velocity commands
 */
void Trajectory::setTargetPos(float _targetPos) {
  target = _targetPos;
  velGoal = maxVel;
  type = 0;
  noTasks = false;
}


/**
 * Set a new target position and time
 *
 * @param  (_targetPos) New target position value
 * @param  (_time) The time in seconds until system should reach target position
 * @return (True) goal is achievable, (False) goal won't be achieved within specified time
 * @note   This overides any previous position or velocity commands
 */
bool Trajectory::setTargetPos(float _targetPos, float _time) {
  target = _targetPos;
  
  // Take into account the cut-off theshold
  _time = _time + sqrt(2 * threshold / dec);

  // Calculate the velocity required to reach the goal within the specified time
  float sqrtTerm = (_time * _time) - 2 * abs(_targetPos - curPos) * (1/acc + 1/dec);

  // If the term is a positive number, then a solution exists
  if (sqrtTerm >= 0) {
    velGoal = (_time - sqrt(sqrtTerm)) / (1/acc + 1/dec);

    if (velGoal > maxVel) velGoal = maxVel;
  } else {
    velGoal = maxVel;
  }

  type = 0;
  noTasks = false;

  if (velGoal == maxVel) return false;
  else return true;
}


/**
 * Set a new target velocity
 *
 * @param  (_targetVel) New target velocity value
 * @note   This overides any previous position or velocity commands
 */
void Trajectory::setTargetVel(float _targetVel) {
  if (_targetVel > maxVel) target = maxVel;
  else if (_targetVel < -maxVel) target = -maxVel;
  else target = _targetVel;
  type = 1;
  noTasks = false;
}


/**
 * Update controller and calculate new position value
 *
 * @param   (dT) The time change (ms) since function was last called
 * @return  The new current position of the system
 */
float Trajectory::update(float dT) {
  if(noTasks) return curPos;
  // Convert ms to seconds
  dT /= 1000.0;

  // Position Control (Linear velocity with parabolic blends)
  if (type == 0) {

    float posError = target - curPos;

    // If position error is above the threshold
    if (abs(posError) > threshold) {

      // Determine motion direction
      bool dir = true;
      if (posError < 0) dir = false;

      // Determine whether to accelerate or decelerate
      float acceleration = acc;
      if ((curVel * curVel / (2 * dec)) >= abs(posError)) acceleration = -dec;

      // Update the current velocity
      if (dir) curVel += acceleration * dT;
      else curVel -= acceleration * dT;

      // Limit Velocity
      if (curVel > velGoal) curVel = velGoal;
      else if (curVel < -velGoal) curVel = -velGoal;
      
      float dP = curVel * dT;

      // Prevent overshooting/jittering around target
      if (abs(dP) < abs(posError)) curPos += dP;
      else curPos = target;

      //Serial.print(millis()); Serial.print(",");
      //Serial.print(curPos); Serial.print(",");
      //Serial.print(curVel); Serial.print(",");
      //if (abs(curVel) != velGoal) Serial.println(acceleration * (-1 + 2*dir));
      //else Serial.println(0);

    } else {
      curVel = 0;
      curPos = target;
      noTasks = true;
    }

  // Velocity Control (Linear velocity with parabolic blends)
  } else if (type == 1) {

    float velError = target - curVel;

    // If velocity error is above the threshold
    if (abs(velError) > threshold) {

      // Determine whether to accelerate or decelerate
      float acceleration = acc;
      if (target < curVel && curVel >= 0) acceleration = -dec;
      else if (target < curVel && curVel < 0) acceleration = -acc; 
      else if (target > curVel && curVel < 0) acceleration = dec;

      // Update the current velocity
      float dV = acceleration * dT;
      if (abs(dV) < abs(velError)) curVel += dV;
      else curVel = target;

    } else {
      curVel = target;
      noTasks = true;
    }
    
    // Limit Velocity
    if (curVel > maxVel) curVel = maxVel;
    else if (curVel < -maxVel) curVel = -maxVel;

    // Update current position
    curPos += curVel * dT;
  }

  // Error check for NaN
  //if (curPos != curPos) {
  //  //printf(F("ERROR: Dynamics calculation error"));
  //  if (type == 0) curPos = target;
  //  else curPos = 0;
  //  curVel = 0;
  //}

  return curPos;
}


/**
 * Overloaded "update" function, were the time change has not been specified
 *
 * @return  The new current position of the system
 */
float Trajectory::update() {
  // Calculate Time Change
  unsigned long newTime = millis();
  float dT = float(newTime - oldTime);
  oldTime = newTime;
  return update(dT);
}



/**
 * Reset all class variables
 *
 * @param (newPos) the current position - default = 0
 */
void Trajectory::reset(float newPos) { 
  curPos = newPos;
  curVel = 0;
  target = 0;
  velGoal = maxVel;
  noTasks = true;
  oldTime = millis();
}


/**
 * Set/reset the current position value
 *
 * @param (newPos) the new current position - default = 0
 */
void Trajectory::setPos(float newPos) {
  curPos = newPos;
}
