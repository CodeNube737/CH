//Test8A.cpp
#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "cvui.h" // for lab4 onward. To be included in .cpp file, not .h file
#include "Test8A.h"

Test8A::Test8A(int scaleFactor = 100) :
   reset(0), quit(0), //global
   guiSize(3 * scaleFactor, 6 * scaleFactor), TopLeft0(0, 0), //gui
   hueMin(0), satMin(110), valMin(153), hueMax(19), satMax(240), valMax(255), //default hsv trackers for orange (why NAT)
   scale(20)// "scale" is a percentage 
{
   _canvas = cv::Mat(guiSize, CV_8UC3); // main window... shrink to be as small as possible
   imgRGB = cv::imread("marbles_1.jpg"); // print the image
   aspectRatio = (double)imgRGB.cols / imgRGB.rows;
   newWidth = 1920 * scale / 100;
   newHeight = newWidth / aspectRatio;
   tmp = cv::Mat::zeros(imgRGB.size(), imgRGB.type()); // tmp
   imgHSV = cv::Mat::zeros(imgRGB.size(), imgRGB.type()); //HSV
   cv::cvtColor(imgRGB, imgHSV, cv::COLOR_BGR2HSV); // HSV
   imgMask = cv::Mat::zeros(imgHSV.size(), imgHSV.type()); // Mask
}

void Test8A::run()
{
   //using namespace cv;
   quit = false;

   std::thread drawThread(&Test8A::draw, this);
   std::thread updateThread(&Test8A::update, this);

   // Ensure the drawThread and updateThread have completed their execution before the next iteration
   drawThread.join();
   updateThread.join();

   while (!quit)
      quit = cv::waitKey(1);
   exit(0);
}

void Test8A::update()
{
   std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_THREAD));
}

void Test8A::draw()
{
   cvui::init(WINDOW_NAME, 20); // init main window
   if (imgRGB.empty())          // check if main window is empty
   {
      std::cout << "Failed to load image from marbles_1.jpg" << std::endl;
      return;
   }

   while (!quit)
   {
      //////////////////////////////////
      //fpsCounter.update(); // Update FPS counter
      float fps = fpsCounter.FPS;
      if (!(fps))
         cvui::printf(_canvas, 120, 80, "Frames/sec: %f", fps);

      //////////////////////////////////
      // setup the menu
      _canvas = cv::Scalar(49, 52, 49); // clear the _canvas
      cvui::window(_canvas, 10, 10, 200, 300, WINDOW_NAME); // menu window... shrink to be as small as possible
      if (cv::waitKey(1) && (cvui::button(_canvas, 10, 30, "&Quit")))
         quit = true;
      /*
      cv::createTrackbar("Hue min", WINDOW_NAME, &hueMin, 179);
      cv::createTrackbar("Hue max", WINDOW_NAME, &hueMax, 179);
      cv::createTrackbar("Sat min", WINDOW_NAME, &satMin, 255);
      cv::createTrackbar("Sat max", WINDOW_NAME, &satMax, 255);
      cv::createTrackbar("Val min", WINDOW_NAME, &valMin, 255);
      cv::createTrackbar("Val max", WINDOW_NAME, &valMax, 255);
      */

      cvui::printf(_canvas, TopLeft0.x + 25, TopLeft0.y + 70 + 25, "Hue min");
      cvui::trackbar(_canvas, TopLeft0.x + 50, TopLeft0.y + 70, 150, &hueMin, (int)0, (int)179);
      cvui::trackbar(_canvas, TopLeft0.x + 50, TopLeft0.y + 140, 150, &hueMax, (int)0, (int)179);
      cvui::trackbar(_canvas, TopLeft0.x + 50, TopLeft0.y + 210, 150, &satMin, (int)0, (int)255);
      cvui::trackbar(_canvas, TopLeft0.x + 50, TopLeft0.y + 280, 150, &satMax, (int)0, (int)255);
      cvui::trackbar(_canvas, TopLeft0.x + 50, TopLeft0.y + 350, 150, &valMin, (int)0, (int)255);
      cvui::trackbar(_canvas, TopLeft0.x + 50, TopLeft0.y + 420, 150, &valMax, (int)0, (int)255);
      lower = cv::Scalar(hueMin, satMin, valMin); // HSV-Mask
      upper = cv::Scalar(hueMax, satMax, valMax); // HSV-Mask
      imgMask = cv::Scalar(0, 0, 0); // clear the mask
      cv::inRange(imgHSV, lower, upper, imgMask); // Redraw Mask

      //////////////////////////////////
      cvui::update(); // ui update
      cv::imshow(WINDOW_NAME, _canvas); // Update the window where the trackbars are

      cv::resize(imgRGB, tmp, cv::Size(newWidth, newHeight));
      cv::imshow("Original Image", tmp);
      cv::moveWindow("Image", 0, 0);

      cv::resize(imgHSV, tmp, cv::Size(newWidth, newHeight));
      cv::imshow("HSV Image", tmp);
      cv::moveWindow("HSV Image", newWidth * 1, 0);

      cv::resize(imgMask, tmp, cv::Size(newWidth, newHeight));
      cv::imshow("Masked Image", tmp);
      cv::moveWindow("Masked Image", newWidth * 2, 0);

      std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_THREAD));
   }
}