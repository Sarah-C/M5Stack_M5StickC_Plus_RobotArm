# M5Stack_M5StickC_Plus_RobotArm
This is a step-by-step, 4 axis, velocity controlled robot arm controller using the Arduino IDE and an M5Stack M5StickC Plus.

It plays back a sequence of arm positions in a loop, smoothly accelerating and decelerating to prevent the robot arm jerking.

![image](https://user-images.githubusercontent.com/1586332/147946409-bb8a676f-3fe4-45bf-9403-eece96cdca8b.png)

**Hardware:**          
A 3D printer. (You can find people who print the models online if you don't have one)                
Shop bought arm: https://www.banggood.com/Small-Hammer-3D-Print-DIY-4DOF-RC-Robot-Arm-Kit-With-SG90-Servos-p-1451689.html                  
3D printer files for robot arm: https://www.thingiverse.com/thing:65081             
M5StickC Plus: https://shop.m5stack.com/products/m5stickc-plus-esp32-pico-mini-iot-development-kit             
I2C C-Back driver board: https://shop.m5stack.com/products/c-back-hat-with-servo-driver-stm32f0            


**For the power supply (4 servos flatten the M5Stick battery quite fast!):**             
Rounded vero-board: https://www.amazon.co.uk/gp/product/B07G5CP6G2/              
Micro USB breakout: https://www.amazon.co.uk/WayinTop-Pinboard-Breakout-Converter-Breadboard/dp/B07W13X3TD/               
Wire up the servo power pins to the USB +5/GND volts as well as the ground of the C-Back driver. Wire up the signal pins from the M5Stick through the board to the servos.                             

**Software:**          
Servo velocity control (already included): https://wired.chillibasket.com/2020/05/servo-trajectory/              
This project.               
