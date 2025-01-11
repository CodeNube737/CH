#pragma once
#include "CBase4618.h"
#include "FPSCounter.h" ///< Include the header for the FPSCounter class
#define WINDOW_NAME "Lab8 Test"

class Test8 : public CBase4618
{
private:
   bool reset; // reset flag
   bool quit; // quit flag
   //int count; // count integer
   FPSCounter fpsCounter; // Create an instance of the FPSCounter class

public:
   Test8(); // constructor
   virtual void run();
   virtual void draw();
   virtual void update();
};