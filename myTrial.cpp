//"myTrial.cpp" - this will not be one functions, but a bunch of functions commented-out so u can choose one to run and test stuff
#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "cvui.h" // lab4 onward, to be included in .cpp file, not .h file
#include "myTrial.h"
#include "CControl.h"

myTrial::myTrial() { } // constructor... idk why, but i see it as necessary

void myTrial::loopTest()
{
   var1 = 0; // note, you can access var1 (private) only cuz ur in member-function loopTest() in myTrial class
   while (!_kbhit())
      std::cout << "Press any key to exit " << var1++ << '\n'; // my tests showed an ~10kHz clock speed
}

void myTrial::LedToggle()
{
   const char SET = 'S';
   bool powerOn = false;

   bool working = false;
   enum type { DIGITAL = 0, ANALOG, SERVO };
   _trialCom.flush(); // note that "_trialCom" is defined in the myTrial class
   working = _trialCom.open("COM3");


   while (working)
   {
      std::cout << "\n\npress \"Ctrl + c\" to exit";
      std::cout << "\nTurn LEDs?\n\t(1) on\n\t(0) off\n\t";
      std::cin >> powerOn;

      for (size_t i = 0; i < 3; i++)
      {
         std::string value =
            std::string(1, SET)
            + " "
            + std::to_string(DIGITAL)
            + " " + std::to_string(LED_B + i)
            + " "
            + (powerOn ? "1" : "0") + "\n";

         _trialCom.flush(); // note that "_trialCom" is defined in the myTrial class
         std::cout << value << '\n';
         _trialCom.write(value.c_str(), value.size()); // write to the serial port to turn on LED
      }

      /*
      char buffer[1024];
      _trialCom.read(buffer, sizeof(buffer)); // read from the serial port
      std::cout << buffer << '\n';
      */
   }
}

void myTrial::TestCom(std::string send) // currently not setup to get "send" passed in
{
   Serial _com;
   _com.open("COM3");

   // Array of strings to send
   std::string stringsToSend[] = { "S 2 0 92", "S 2 0 180", "S 2 0 1", "S 2 0 180", "S 2 0 92" };

   // Buffer to hold the response
   char buffer[256];

   // Send each string in the array
   for (const auto& str : stringsToSend) {
      // Write the string to the COM port
      _com.write(str.c_str(), str.size());

      std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second

      // Read the response
      int bytesRead = _com.read(buffer, sizeof(buffer) - 1);

      // Null-terminate the string
      buffer[bytesRead] = '\0';

      // Parse the received data
      std::stringstream ss(buffer);
      std::string temp;
      int toUse; // the following while-loop gets the 'result' portion of the buffer, since the type and channel should't change
      while (ss >> temp) {
         if (std::stringstream(temp) >> toUse) {
            // Successfully parsed an integer, do nothing
         }
      }

      // Print the response
      std::cout << "Received: " << buffer << "; to use: " << toUse << std::endl;
   }
}

std::string test() { return "testing\n"; } // just made to see if this syntax compiles

void myTrial::myPoint(int& x, int& y) // if passing the point vector to other functions, u always need variables x & y declared.
{
   //Point_<int> point;  // Chat GPT says it creates a Point_ object with x and y initialized to 0
   cv::Point Point_;

   Point_ = ( 5 * cv::Point(x, y) + cv::Point(1, 1) ); // line that showcases all the arithmetic that can be done to a point
   std::cout << Point_.x << ", " << Point_.y << std::endl;

   //check: this function should always print "(1, -1)" before it is exited from
   Point_ = cv::Point(1    ,    // it seems the Point type (point2) ignores all white-space, as can bee seen by running this funky line
      -1        );
   std::cout << Point_.x << ", " << Point_.y << std::endl;
}

void T_update() //i currently don't have a purpose for this 
{

}

void myTrial::T_draw() // pretty much the pre-lab for lab 4
{
   cvui::init(WINDOW_NAME, 20); // the ", 20" was added once the exit button was added
   cv::Mat frame = cv::Mat(cv::Size(650, 150), CV_8UC3);
   cv::Point gui_position;
   std::vector<double> values;

   gui_position = cv::Point(10, 10);

   while (true) {
      frame = cv::Scalar(49, 52, 49);

      cvui::text(frame, 40, 40, "To exit this app click the button below or press Q (shortcut for the button below).");

      // Exit the application if the quit button was pressed.
      // It can be pressed because of a mouse click or because 
      // the user pressed the "q" key on the keyboard, which is
      // marked as a shortcut in the button label ("&Quit").
      if (cvui::button(frame, 300, 80, "&Quit")) {
         break;
      }

      // Since cvui::init() received a param regarding waitKey,
      // there is no need to call cv::waitKey() anymore. cvui::update()
      // will do it automatically.
      cvui::update();

      Sparky(frame); // can be commented-out if u don't want the Sparkline, but MUST go *before* imshow()
      cv::imshow(WINDOW_NAME, frame);
   }
}

void myTrial::Test_joy()
{
   CControl control;
   float p = 0;
   cv::Point pt = cv::Point(1, 0);
   control.init_com(3);

   while (true)
   {
      p = control.get_joystick(1, pt);
      std::cout << pt << ";\t" << p << "%\n";
   }
}

void myTrial::Sparky(cv::Mat frame)
{
   std::vector<double> values;
   for (std::vector<double>::size_type i = 0; i < 30; i++) {
      values.push_back(rand() + 0.);
   }

   cvui::sparkline(frame, values, 10, 10, 280, 100);
}

// eventually, myTrial can be improved to have its own menu and switch function (below, or above this comment)... or wtvr

void myTrial::T_draw2()
{
   Ball ball(0.5, 0.5, 0.01, 0.01); // let's work some magic

   int scaleFactor = 100;
   cv::Point guiSize(10 * scaleFactor, 8 * scaleFactor); // (width, height)
   cv::Point paddleSize(20, 150); // (width, height)
   cv::Point gui_position;
   cv::Point ballCenter(guiSize.x/2, guiSize.y/2);// center of the gui ... add the x and y of the ball??
   double radius = 40;
   int playerScore = 0;
   int PonglordScore = 0;

   cvui::init(WINDOW_NAME, 20); // supposedly, includes the "hit 'q' to exit bit
   cv::Mat frame = cv::Mat(cv::Size(guiSize.x, guiSize.y), CV_8UC3); // (size(width, height), type)
   gui_position = cv::Point(10, 10);


   while (true) {

      frame = cv::Scalar(49, 52, 49); // clear the frame
      cvui::window(frame, 10, 10, 200, 200, "Pong"); // redraw the window

      if (cvui::button(frame, 10, 40, "&Quit")) {

         frame = cv::Scalar(0, 0, 0); // Clear the frame. You can change the color as needed

         cvui::printf(frame, 10, 60, "Game stopped. Press 'q' to quit");// Print the message
         cvui::update();// Update the frame
         cv::imshow(WINDOW_NAME, frame);// Show the updated frame

         break;
      }

      int i = 0;
      ballCenter = cv::Point(guiSize.x / 2 + i, guiSize.y / 2 + i);

      // draw the gui & its elements based on constructor's variables
      cvui::rect(frame, 0 + 10, (guiSize.y - paddleSize.y) / 2 + i, paddleSize.x, paddleSize.y, 0xffffff, 0xffffff); // 'frame', x, y, width, height, border color, fill color
      cvui::rect(frame, guiSize.x - 10 - paddleSize.x, (guiSize.y - paddleSize.y) / 2, paddleSize.x, paddleSize.y, 0xffffff, 0xffffff); // 'frame', x, y, width, height, border color, fill color
      cv::circle(frame, ballCenter, radius, cv::Scalar(0xff, 0xff, 0xff), CVUI_FILLED, 8, 0); // (i/o-array, centre(Pt), radius, color(Scalar), thickness or filled, lineType, shift)
      cvui::printf(frame, 25, 70, "Score");
      cvui::printf(frame, 25, 85, "Player: %d", playerScore);
      cvui::printf(frame, 25, 100, "Ponglord: %d", PonglordScore);
      cvui::trackbar(frame, 0 + 15, 120, 200 - 15, &radius, (double)5.0, (double)100.0);
      cvui::update();
      cv::imshow(WINDOW_NAME, frame);

      i++;
   }

}

void myTrial::Ballon()
{
   Ball ball(0.5, 0.5, 0.01, 0.01);

   while (true) {
      ball.update(0.01);

      // Clear console and print ball position
      std::cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear screen and move cursor
      std::cout << "Ball is at (" << ball.x << ", " << ball.y << ")\n";

      // Wait for a bit
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }
}