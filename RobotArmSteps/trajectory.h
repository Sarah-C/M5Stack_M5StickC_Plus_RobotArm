/* * * * * * * * * * * * * * * * * * * * * * *
 * TRAJECTORY CONTROLLER CLASS
 *
 * Code by: Simon Bluett
 * Website: https://wired.chillibasket.com
 * Version: 1.2
 * Date:    19th May 2020
 * Copyright (C) 2020, MIT License
 *
 * This class can be used to control the movement and velocity of 
 * servo motors or DC motors with encoders. When a new target 
 * position is set, it accelerates at a constant rate until a
 * maximum velocity is reached. As the system approaches the
 * target position, it then smoothly decelerates to a stop.
 *
 * This controller uses a simple trapezoidal velocity profile.
 *
 * For more information, please visit my tutorial at:
 * https://wired.chillibasket.com/2020/05/servo-trajectory
 *
 * An example Arduino sketch is also provided with this library
 * to show how the class can be used in practice.
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <Arduino.h>

// TRAJECTORY CLASS
class Trajectory {

public:
	// Constructors
	Trajectory(float _maxVel = 100, float _setAcc = 50, float _setDec = -1, float _thresh = 0.1);

	// Set target position/velocity
	void setTargetPos(float _target);
	bool setTargetPos(float _target, float _time);
	void setTargetVel(float _target);

	// Get the current target position or velocity values
	float getTarget() { return target; };

	// Set max velocity, acceleration or decelleration
	void setMaxVel(float _maxVel) { maxVel = _maxVel; };
	void setAcc(float _acc) { acc = _acc; };
	void setDec(float _dec) { dec = _dec; };

	// Get the current max velocity, accel or decel values 
	float getMaxVel() { return maxVel; };
	float getAcc() { return acc; };
	float getDec() { return dec; };

	// Get the current position and velocity
	float getPos() { return curPos; };
	float getVel() { return curVel; };

	// Set the current position
	void setPos(float newPos = 0);
	
	// Reset the controller
	void reset(float newPos = 0);

	// Check if the controller has reached the desired position
	bool ready() { return noTasks; };

	// Update and get the new position value
	// dT = time since update or reset function was last called in milliseconds
	float update(float dT);
	float update();

	// Default destructor
	~Trajectory(); 

private:
	// type = 0: "position" control (Straight line with parabolic blends)
	// type = 1: "velocity" control (Straight line with parabolic blends)
	int type;          // Target type

	float threshold;   // Position error threshold below which no motion occurs 
	float target;      // Target Position or Velocity
	float curPos;      // Current Position (units)
	float curVel;      // Current Velocity (units/second)
	float maxVel;      // Maximum Velocity (units/second)
	float velGoal;     // Target velocity to reach goal position at a specific time
	float acc;         // Constant Acceleraction (units/second^2)
	float dec;         // Constant Deceleration (units/second^2)
	unsigned long oldTime;
	bool noTasks;
};

#endif /* TRAJECTORY_H */
