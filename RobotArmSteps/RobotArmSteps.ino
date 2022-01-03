#include <Arduino.h>
#include <M5StickCPlus.h>
#include "RobotArmMover.h"
#define totalMoves 17


RobotArmMover ram;
int8_t moveNumber = 0;

uint8_t moves[totalMoves][4] = {

  {175,   5, 110, 100}, // Rotate left from upright, open gripper
  {175,  50, 175, 100}, // Drop arm to first object
  {175,  50, 175,  48}, // Close gripper
  {175,   5,  70,  48}, // Point arm up
  {  5,   5,  70,  48}, // Rotate to the drop position
  {  5,  70, 130,  48}, // Drop down to position 
  {  5,  70, 130, 100}, // Open arm
  {  5,   5,  70, 100}, // Point arm up
  {175,   5,  70, 100}, // Rotate left from upright, open gripper
  {175,  75, 130, 100}, // Drop down to second block
  {175,  75, 130,  48}, // Close gripper
  {175,   5,  70,  48}, // Point arm up
  {  5,   5,  70,  48}, // Rotate to drop position
  {  5,  55, 145,  48}, // Drop down to position 
  {  5,  55, 145, 100}, // Open gripper 
  {  5,   5,  70, 100}, // Straighten up

  { 90,   5, 110,  20}
  
};



void setup() {
  M5.begin();
  M5.Lcd.setTextColor(TFT_GREEN);
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawString("Robot arm running.", 10, 5, 2);
  ram.init();
  //ram.setSpeed(10);
}

void nextMove() {
  Serial.print("Making move: ");
  Serial.println(moveNumber);
  
  ram.setBase(moves[moveNumber][0]);
  ram.setLowerArm(moves[moveNumber][1]);
  ram.setUpperArm(moves[moveNumber][2]);
  ram.setGripper(moves[moveNumber][3]);
  if(++moveNumber == totalMoves) {
    Serial.println("Parking");   
    ram.park();
    moveNumber = 0;
  }
}

void loop() {
  char buffer[30];

  ram.update();

  M5.Lcd.drawString("Base: ", 10, 25, 2);

  snprintf(buffer, sizeof(buffer), "%.2f", ram.getBase());
  M5.Lcd.fillRect(60, 25, 100, 15, 0);
  M5.Lcd.drawString(buffer, 60, 25, 2);

  snprintf(buffer, sizeof(buffer), "%.2f", ram.getForearm());
  M5.Lcd.fillRect(60, 40, 100, 15, 0);
  M5.Lcd.drawString(buffer, 60, 40, 2);

  snprintf(buffer, sizeof(buffer), "%.2f", ram.getArm());
  M5.Lcd.fillRect(60, 55, 100, 15, 0);
  M5.Lcd.drawString(buffer, 60, 55, 2);

  snprintf(buffer, sizeof(buffer), "%.2f", ram.getGripper());
  M5.Lcd.fillRect(60, 70, 100, 15, 0);
  M5.Lcd.drawString(buffer, 60, 70, 2);

  delay(1);

  if (moveNumber != -1) {
    if (ram.ready()) {
      nextMove();
    }
  }
}
