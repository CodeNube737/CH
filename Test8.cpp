#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "cvui.h" // for lab4 onward. To be included in .cpp file, not .h file
#include "Test8.h"

Test8::Test8() :
   reset(0), quit(0) //global
{

}

void Test8::run()
{
   quit = false;

   while (!quit)
   {
      draw();
      int key = cv::waitKey(0); // wait indefinitely for a key press
      quit = key != -1; // quit becomes true if a key is pressed
   }
   exit(0);
}

void Test8::draw()
{
   cv::namedWindow("Trackbars", (640, 200));

   cvui::init(WINDOW_NAME, 20);

   cv::Mat imgHSV, mask;
   int hmin = 0, smin = 110, vmin = 153;
   int hMax = 19, sMax = 240, vMax = 255;
   cv::Mat img = cv::imread("marbles_1.jpg");
   if (img.empty())
   {
      std::cout << "Failed to load image from marbles_1.jpg" << std::endl;
      return;
   }
   cv::createTrackbar("Hue min", "Trackbars", &hmin, 179);
   cv::createTrackbar("Hue max", "Trackbars", &hMax, 179);
   cv::createTrackbar("Sat min", "Trackbars", &smin, 179);
   cv::createTrackbar("Sat max", "Trackbars", &sMax, 179);
   cv::createTrackbar("Val min", "Trackbars", &vmin, 179);
   cv::createTrackbar("Val max", "Trackbars", &vMax, 179);

   // Resize the images to fit half the screen width while maintaining aspect ratio
   double aspectRatio = (double)img.cols / img.rows;
   int newWidth = 1920 / 2;
   int newHeight = newWidth / aspectRatio;
   cv::resize(img, img, cv::Size(newWidth, newHeight));

   cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
   cv::resize(imgHSV, imgHSV, cv::Size(newWidth, newHeight));

   cv::Scalar lower(hmin, smin, vmin);
   cv::Scalar upper(hMax, sMax, vMax);
   cv::inRange(imgHSV, lower, upper, mask);

   // Display the canvas
   cv::imshow("Image", img);
   cv::imshow("Image HSV", imgHSV);
   cv::imshow("IMmage Mask", mask);

   //if (cv::waitKey(1) && (cvui::button(_canvas, 10, 40, "&Quit")))
   //   quit = true;
}

void Test8::update()
{
   //fpsCounter.update(); // Update FPS counter
}