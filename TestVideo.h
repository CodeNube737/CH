//TestVideo.cpp
#pragma once
#include "CBase4618.h"

class TestVideo
{
public:
	TestVideo(); // constructor
	//~TestVideo(); // destructor
	void do_video();

   // Define constants for magic numbers... note, these are not useful as Pre-processor definitions
   const int GAUSSIAN_BLUR_SIZE;
   const double GAUSSIAN_BLUR_SIGMA;
   const int CANNY_THRESHOLD1;
   const int CANNY_THRESHOLD2;
   const int CANNY_APERTURE_SIZE;
};
