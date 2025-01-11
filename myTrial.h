#pragma once
// "myTrial.h" is a file used for personal trials just if you want to see how things work.

// Add simple GUI elements
#define WINDOW_NAME "CVUI Test"
#define WINDOW1_NAME "CVUI Test 1"

#include "Serial.h"

class myTrial 
{
private:
   Serial _trialCom; // Serial object used for communication. 
   int var1 = 0, var2 = 0;
   char char1 = 0, char2 = 0;

public:
   myTrial(); // constructor
   void loopTest();
   void LedToggle();
   void TestCom(std::string send);
   void myPoint(int& x, int& y);
   void T_draw();
   void T_draw2();
   void Test_joy();
   void Sparky(cv::Mat frame);
   void Ballon();

};

class Ball {
public:
   double x, y; // position
   double vx, vy; // velocity

   Ball(double x, double y, double vx, double vy)
      : x(x), y(y), vx(vx), vy(vy) {}

   void update(double dt) {
      x += vx * dt;
      y += vy * dt;

      // Bounce off walls
      if (x < 0 || x > 1) vx = -vx;
      if (y < 0 || y > 1) vy = -vy;
   }
};