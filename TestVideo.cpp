//TestVideo.cpp

/*
* NOTE: Some webcams load very slowly which may need:
* vid.open(0, CAP_DSHOW);
*
*And on ***LINUX*** you may need:
* vid.open(0, CAP_V4L2);
*/
#include "stdafx.h"
#include "cvui.h"
#include "TestVideo.h"

TestVideo::TestVideo() :
   // Define constants for magic numbers
   GAUSSIAN_BLUR_SIZE(7),
   GAUSSIAN_BLUR_SIGMA(1.5),
   CANNY_THRESHOLD1(0),
   CANNY_THRESHOLD2(30),
   CANNY_APERTURE_SIZE(3)
{ }

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void TestVideo::do_video()
{
   cv::VideoCapture vid;
   vid.open(0, cv::CAP_DSHOW); //vid.open(0); // "cv::CAP_DSHOW" prevents certain errors from occuring on my laptop
   if (!vid.isOpened())
   {
      std::cerr << "Error: Could not open video capture" << std::endl;
      return;
   }

   std::cout << "\n Press [space] to exit";

   do
   {
      cv::Mat frame, edges;
      vid >> frame;
      if (!frame.empty())
      {
         // Convert the frame to grayscale
         cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);

         // Blur the image to reduce noise
         cv::GaussianBlur(edges, edges, cv::Size(GAUSSIAN_BLUR_SIZE, GAUSSIAN_BLUR_SIZE), GAUSSIAN_BLUR_SIGMA, GAUSSIAN_BLUR_SIGMA);

         // Detect edges in the image
         cv::Canny(edges, edges, CANNY_THRESHOLD1, CANNY_THRESHOLD2, CANNY_APERTURE_SIZE);

         // Display the edges
         cv::imshow("edges", edges);
      }
   } while (cv::waitKey(10) != ' ');

   // Release the video capture
   vid.release();
}