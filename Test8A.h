//Test8A.h
#pragma once
#include "CBase4618.h"
#include "FPSCounter.h" // Include the header for the FPSCounter class
#define WINDOW_NAME "Colour Controller 8A"
#define DELAY_THREAD 1 // the Higher this is, the clunkier your ui, but the less CPU it takes to run threads (Imma stop relying fpsDelay)
//*****// NOTE: the above delay will defs cause bad lag... when running servos on your Pi... be careful, be aware.

class Test8A : public CBase4618
{
private:
   bool reset; // reset flag
   bool quit; // quit flag
   FPSCounter fpsCounter; // Create an instance of the FPSCounter class

public:
   Test8A(int); // constructor

   // crime: public vars with no getters and setters:
   cv::Mat imgRGB, imgHSV, imgMask, tmp;
   cv::Size guiSize; // proportional to scalefactor
   cv::Point TopLeft0; // the origin of cvui windows
   int hueMin, satMin, valMin;
   int hueMax, satMax, valMax;
   int scale;
   int newHeight, newWidth;
   double aspectRatio;
   cv::Scalar lower;
   cv::Scalar upper;

   // functions:
   virtual void run();
   virtual void draw();
   virtual void update();
};