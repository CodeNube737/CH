//FINAL fsht ... I added all relevant information from labs 1-9, lecs 1-18... edit lab 6, move to Word
/////////////////////////////////////////////////////////////lab2
#define final_weight 0.30
class CStudent { 
private: 
	std::string snum; 
	float lab; 
	float quiz; 
	float MT; 
	float Fexam; 
	float FinalGrade; 
public: 
	CStudent(std::string asnum, float alab, float aquiz, float aMT, float aFexam, float aFinalGrade); 
	~CStudent(); 
	void setsnum(const std::string& asnum); 
	std::string getsnum() const; 
	void setlab(const float& alab); 
	float getlab() const; 
	void setquiz(const float& aquiz); 
	float getquiz() const; 
	void setMT(const float& aMT); 
	float getMT() const; 
	void setFexam(const float& aFexam); 
	float getFexam() const;
	void setFinalGrade(const float& aFinalGrade); 
	float getFinalGrade() const; };
class CCourse {
private:
   std::vector<CStudent> Course1; // ***VECTOR***
public:
   CCourse();
   float getValidNumericInput();
   float getValidStudentIndex();
   std::string getValidStudentNumber();
   void add_student();
   void edit_student();
   void delete_student();
   void print_grades();
   void load_class();
   void save_class();
   std::string memoryFileType(); // returns either 'J' or 'T'};

   std::string CCourse::getValidStudentNumber() {
      std::regex pattern("(A00|A01)\\d{6}");  // A00 or A01 followed by exactly 6 digits // beautifully done
      std::string snum;
      while (true) {
         cin >> snum;
         cout << "\n";
         if (regex_match(snum, pattern)) {
            return snum;
            break;
         }
         else
            cout << "Invalid number" << endl;
      }
   }
};
void CCourse::delete_student() {
   int i;
   if (!Course1.size()) // if no students to the vector Course1 (size zero), u can't edit any students
      cout << "\nThere are no sutdents to delete\n\n";
   else {
      cout << "\nStudent to Delete: ";
      i = getValidStudentIndex() - 1;
      if (i >= 0 && i < Course1.size()) {
         Course1.erase(Course1.begin() + i);
         cout << "\nStudent deleted successfully." << endl;
      }
   }
}            
void CCourse::load_class() {
    Course1.clear(); //this clears the CCourse1 vector so that when you load a file, none of the previous info is
    std::string input;
    cout << "\nName of Load File (ignore .type): ";
    cin >> input;
    input += memoryFileType();
    std::ifstream inputFile(input);
    if (inputFile.is_open()) {
       std::string snum;
       float lab, quiz, MT, Fexam, FinalGrade;
       while (inputFile >> snum >> lab >> quiz >> MT >> Fexam >> FinalGrade) { //notice, this is in the exact same order as outfile()
          Course1.push_back(CStudent(snum, lab, quiz, MT, Fexam, FinalGrade));
       }
       inputFile.close();

       // Remove the file after loading its content
       if (remove(input.c_str()) != 0)
          std::cerr << "Error loading the file!" << std::endl;
       else
          std::cout << "File successfully loaded." << std::endl;
    }
    else 
        std::cerr << "Error loading the file!" << std::endl;}
void CCourse::save_class() {          //this will write to an external file
    int track = Course1.size();
    std::string output;
    cout << "\nName of Save File (ignore .type): ";
    cin >> output;
    output += memoryFileType();
    ofstream outfile(output); //,ios::app add this in the brackets to only add data
    for (int i = 0; i < track; i++) {
        if (outfile.fail())
            cout << "file error" << endl;       
        else
        {
            outfile << Course1[i].getsnum() << " "
                << Course1[i].getlab() << " "
                << Course1[i].getquiz() << " "
                << Course1[i].getMT() << " "
                << Course1[i].getFexam() << " "
                << Course1[i].getFinalGrade() << "\n"; /// preferred cout in a new line
        }
    } cout << "file was written" << endl;}
/////////////////////////////////////////////////////////////lab3... commented is energia
//#include <Servo.h>
// void setup()
// Serial.begin(BAUD_RATE);
// pinMode(digital_index, INPUT_PULLUP);
// digitalWrite(RGBLED_RED_PIN, LOW); /// default keep it OFF (active lo)
// pinMode(SERVO_PORT0, OUTPUT); /// good to know (TYP)
// myservo[0].attach(SERVO_PORT0)
// unsigned long currentMillis = millis();
// char ch = Serial.read();
// value = analogRead(A_PIN[channel]);
// value = myservo[channel].read(); // or write
   /**
    * @file CAsteroidGame.h
    * @author Mikhail Rego
    * @date 2024/Feb/29    ...   (above stuff was the beginning of the file) */
   /**
    * @brief Writes to u-controller to set result for a certain channel.
    * @param type The type of the data.
    * @param channel The channel number.
    * @param val The value to set.
    * @return True if the operation is successful, false otherwise.
    */
bool set_data(int type, int channel, int val);
int channel_digital = 33; ///< The digital channel number.
bool CControl::portSearch(int comport)
{
   do
   {
      for (int i = 0; i < 20; i++)
      {
         init_com(i);
         if (_com.is_open()) { return true; }
      }
   } while (true);
}
bool CControl::set_data(int type, int channel, int val) {
   // Format the data to be sent
   std::stringstream ss;
   ss << "S " << type << " " << channel << " " << val << "\n";
   std::string buffer = ss.str();

   // Write the data to the serial port
   int bytesWritten = _com.write(buffer.c_str(), buffer.length());

   // Check if the write was successful
   return (bytesWritten == buffer.length());
}
/////////////////////////////////////////////////////////////lab4 (1st 5 lines is CBase)
#include "stdafx.h"
#define CVUI_DISABLE_COMPILATION_NOTICES // lab4 onward
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "CBase4618.h" // end of .cpp file
class CBase4618 {
protected:
   CControl control; ///< Instantiate microcontroller.
   cv::Mat _canvas; ///< Make a GUI object called _canvas.
public:
   virtual void update() = 0;
   virtual void draw() = 0;
   virtual void run() = 0;}; // end of .h file
/////////////////////////////////////////////////////////////lab5
class CPong : public CBase4618
{ // skip stuff... 
public: // skip stuff... methods in CPong.h:
   void updatePaddles(float&, float&);
   void updateBall(cv::Point2f&);
   float randomServe();
   void updateScore(float&, int&, bool&, int&, int&);
   virtual void draw();
   virtual void update();
   virtual void run(); // pretty much end of CPong class
};// now start CPong.cpp
#include "stdafx.h" // must be added above all code in external .ccp files w/ template
#include "cvui.h" // for lab4 onward. To be included in .cpp file, not .h file
#include "CBase4618.h"
#include "CPong.h" 
CPong::CPong(int scaleFactor = 100) :
   //global
   reset(0), quit(0), count(0), rand1(1), rand2(1),// win(false),
   //gui
   guiSize(10 * scaleFactor, 8 * scaleFactor), TopLeft0(0, 0),
   //default
   DefaultBall(guiSize.x / 2, guiSize.y / 2), DefaultPlayer(guiSize.x - scaleFactor / 10, guiSize.y / 2)
{} // i skipped a lot of initialization
void CPong::run()
{
   quit = false;
   std::thread drawThread(&CPong::draw, this);
   std::thread updateThread(&CPong::update, this);
   // Ensure the drawThread and updateThread have completed their execution before the next iteration
   drawThread.join();
   updateThread.join();
   while (!quit)
      quit = waitKey(1);
   exit(0);
}
void CPong::update() // should just get the analog(y) value, then be called in the constructor program
{
   while (!quit)
   {
      fpsCounter.update(); // Update FPS counter
      if (!reset)
      {
         if (count++) // updates
         {
            updateBall(ballCentre);
            updatePaddles(playerCenter.y, PonglordCenter.y);
            updateScore(ballCentre.x, count, reset, playerScore, PonglordScore);
         }
         else // skips updates and resets to initial conditions
         {
            rand1 = randomServe();
            rand2 = randomServe();
            // reset to default
            ballDirection = cv::Point2f(rand1, rand2); oldPosition = DefaultBall; ballCentre = DefaultBall; 
            playerCenter = DefaultPlayer; PonglordCenter = DefaultPonglord; //paddles
            oldPosition = playerCenter - ballDirection;
            // now proceed as usual
            updateBall(ballCentre);
            updatePaddles(playerCenter.y, PonglordCenter.y);
            std::cout << "New Game!\n";
         }
      }
      if (reset)
      {
         if ((playerScore == 5))// win??
            std::cout << "winner!\n";
         else
            std::cout << "loser!\n";
      }
   }
}
void CPong::draw() {
   cvui::init(WINDOW_NAME, 20);
   cv::Mat _canvas = cv::Mat(cv::Size(guiSize.x, guiSize.y), CV_8UC3); // (size(width, height), type)
   while (!quit) {
      if (count == MAXINT) { count = 1; }
      fpsCounter.update(); // Update FPS counter
      _canvas = cv::Scalar(49, 52, 49); // clear the _canvas
      cvui::window(_canvas, 10, 10, 200, 300, "Pong"); // redraw the window
      if (cv::waitKey(1) && (cvui::button(_canvas, 10, 40, "&Quit")))
         quit = true;
      if ((cvui::button(_canvas, 110, 40, "Reset")))
      {
         count = 0;
         reset = 0;
         playerScore = 0;
         PonglordScore = 0;
      } // i'm about to skip a bunch of sht...
      cvui::rect(_canvas, x, y, width, height, 0xffffff, 0xffffff); // border color, fill color
      cv::circle(_canvas, ballCentre, ballRadius, cv::Scalar(0xff, 0xff, 0xff), CVUI_FILLED);
      cvui::trackbar(_canvas, 0 + 50, 110, 200 - 50, &ballRadius, (double)5.0, (double)100.0);
      cvui::printf(_canvas, 25, 160, "Frames/sec: %f", fpsCounter.FPS);
      cvui::update();
      cv::imshow(WINDOW_NAME, _canvas);
   }
}
void CPong::updateBall(cv::Point2f& ballPosition)
{ //skipping a bunch...
   ballvelocity = ballSpeed * ballDirection;
   newPosition = ballPosition + ballvelocity * t;
   ballDirection = ((newPosition - oldPosition) /
      sqrt((newPosition.x - oldPosition.x) * (newPosition.x - oldPosition.x)
         + (newPosition.y - oldPosition.y) * (newPosition.y - oldPosition.y))); // v^ = s^ 
   oldPosition = ballPosition; // initialized in constructor to the ball's default position
   ballPosition = newPosition;
   if ((ballBottLeft.x <= PonglordBottRight.x) & ((ballTopLeft.y <= PonglordBottRight.y) & (ballBottLeft.y >= PonglordTopRight.y)))
   {
      oldPosition = newPosition + -1 * ballvelocity;
      newPosition = oldPosition + cv::Point2f(-1 * ballvelocity.x, ballvelocity.y);
      ballDirection = cv::Point2f(-1 * ballDirection.x, 1 * ballDirection.y);
      ballPosition = newPosition;
   }//skipped a bunch of stuff again
}//skipped a bunch of stuff again
float CPong::randomServe() // returns a random number between - 1 and 1
{
   auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
   std::mt19937 generator(seed);
   std::uniform_real_distribution<float> distribution(-1.0f, 1.0f); // Generate a random number between -1 and 1
   return distribution(generator);
}
void CPong::updateScore(float& ballPositionX, int& Count, bool& reset, int& player, int& Ponglord)
{
   if (ballPositionX < (TopLeft0.x + ballRadius / 2))
   {
      player++;
      Count = 0;
   }
   if (ballPositionX > (guiSize.x - ballRadius / 2))
   {
      Ponglord++;
      Count = 0;
   }
   if ((playerScore == winningScore) | (PonglordScore == winningScore))
      reset = 1;
}
/////////////////////////////////////////////////////////////lab6
void CAsteroidGame::draw() {
   cvui::init(WINDOW_NAME, 20); // init main window
   while (!quit) {
      auto startTime = std::chrono::high_resolution_clock::now(); // start FPS timer
      // setup the menu
      _canvas = cv::Scalar(49, 52, 49); // clear the _canvas
      if (cv::waitKey(1) && (cvui::button(_canvas, 10, 10, "&Quit ")))
         quit = true;
      if (cv::waitKey(1) && (cvui::button(_canvas, 10, 40, "&Reset")))
         reset = true;
      cvui::printf(_canvas, 10, 70, "F.P.S. = %f", fps);

      cvui::update(); // ui update
      cv::imshow(WINDOW_NAME, _canvas); // Update the window where the trackbars are

      // setup fps and delay
      std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_THREAD));
      auto endTime = std::chrono::high_resolution_clock::now();
      u_int duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
      fps = 1000.0 / duration; // convert duration to seconds before calculating fps
   }
}
/////////////////////////////////////////////////////////////lab7
///// CControlPi.h /////
#pragma once
#include "CControlPi.h"
#include <chrono>
#include <cmath> // for calculating pwm
#include <iostream>
#include <pigpio.h>

class CControlPi {
private:
   bool _working = false;
public:
   CControlPi();
   ~CControlPi();

   void init(); // initialization in constructor, so this isn't needed anymore
   bool get_digital(int gpioPin, bool& result); // gets a boolean value from a PiGpio pin; gpioSetMode before calling
   bool set_digital(int gpioPin, bool state); // sets a boolean value to a PiGpio pin; gpioSetMode before calling
   float get_analog(); // implements SPI for the MCP3008 ADC; gpioSetMode before calling
   bool set_servo_angle(int gpioPin, int angle); // sets a PWM to a PiGpio pin based on angle 1-180; gpioSetMode before calling
};
///// CControlPi.cpp /////
CControlPi::CControlPi() {}

CControlPi::~CControlPi() {
   if (_working) 
      gpioTerminate(); // Proper pigpio kill
}
void CControlPi::init() {
   if (gpioInitialise() < 0)
   {
      std::cerr << "Failed to initialize pigpio library\n";
      _working = false;
   }
   else
      _working = true;
}
bool CControlPi::get_digital(int gpioPin, bool& result) {
   if (!_working) { return false; }
   result = gpioRead(gpioPin); // Read GPIO pin
   return true; // peripheral read successful
}
bool CControlPi::set_digital(int gpioPin, bool state) {
   if (!_working) { return false; }
   gpioWrite(gpioPin, state ? 1 : 0); // Set the state of the specified GPIO pin
   return true; // peripheral read successful
}
float CControlPi::get_analog() {
   float read_val;
   unsigned char inBuf[3];
   char cmd[] = { 1, 128, 0 }; // 0b1XXX0000 where XXX=channel 0
   int handle = spiOpen(0, 200000, 3); // Mode 0, 200kHz

   spiXfer(handle, cmd, (char*)inBuf, 3); // Transfer 3 bytes
   read_val = ((inBuf[1] & 3) << 8) | inBuf[2]; // Format 10 bits
   read_val /= 1024;
   read_val *= 100;

   spiClose(handle); // Close SPI system

   return read_val;
}
bool CControlPi::set_servo_angle(int gpioPin, int angle) {
   if (!_working) { return false; }
   // Convert angle to pulse width
   // Adjust the min and max pulse width values if necessary for your servo
   const int minPulseWidth = 500; // Pulse width for 0 degrees
   const int maxPulseWidth = 2500; // Pulse width for 180 degrees
   angle = std::max(0, std::min(angle, 180)); // Ensure the angle is within bounds
   // Calculate pulse width
   int pulseWidth = minPulseWidth + (maxPulseWidth - minPulseWidth) * angle / 180;
   gpioServo(gpioPin, pulseWidth);

   return true; // peripheral read successful
}
///// TestControl.h (function definitions in main.cpp needed)... tests digi and anal i/o on pigpio. /////
#pragma once
#include "CControlPi.h"

CControlPi controls;

class TestControlPi //: public CControlPi
{
protected:
   bool quit; // currently unused
   bool digi_1;
   bool digi_2;
   int servo_angle;
   float analog_percent;
public:
   TestControlPi() :
      quit(false), digi_1(false), digi_2(false), servo_angle(0), analog_percent(0)
   {
      controls.init();
   }
   void digitalTest()// digital test function (with on switch input and led output)
   {
      // initialize input and output ports
      int LEDPort = 21; // o/p
      int SWitchPort = 20; // in/p
      int quitPort = 4; // in/p
      // 2 separte variables for their state are digi_1 & _2
      // set to correct modes
      gpioSetMode(LEDPort, PI_OUTPUT); // set pin #21 as an output
      gpioSetMode(SWitchPort, PI_INPUT); // set pin #20 as an input
      gpioSetMode(quitPort, PI_INPUT); // set pin #4 as an quit

      quit = false;

      while (!quit)
      {
         if (controls.get_digital(SWitchPort, digi_1))
            digi_2 = digi_1;
         if (controls.set_digital(LEDPort, digi_2))
            std::cout << "\nLED and switch status: " << digi_2 << ", " << digi_2;
         if (controls.get_digital(quitPort, quit)) {}
      }
   }
   // analog test function (reads ADC input, outputs to servo... see analog_test in CControl)
   void analogTest()
   {
      // initialize inout and output ports
      const int ServoPort = 16; // o/p
      const int quitPort = 4; // in/p
      //int spi = 0; // not needed...spiOpen does all this for u
      // set to correct modes
      gpioSetMode(ServoPort, PI_OUTPUT); // set pin #16 as an output
      gpioSetMode(quitPort, PI_INPUT); // set pin #4 as an quit

      quit = false;
      float inputPercent = 0;
      int outputAngle = 0;

      while (!quit)
      {
         inputPercent = controls.get_analog();
         outputAngle = 180 * (int)inputPercent / 100;
         std::cout << "\nAnalog = " << inputPercent << "%, Angle = " << outputAngle;
         if (controls.set_servo_angle(ServoPort, outputAngle)) {}

         if (controls.get_digital(quitPort, quit)) {}
      }
   }
   void GPIOquit()
   {
      controls.~CControlPi(); // Cleanup and terminate pigpio
      exit(0);
   }
};
// goes in "other complr opt" (lec 11a)
`pkg-config --cflags opencv4` - std = c++11
// goes in "other linker opt" (lec 11a)
`pkg-config --libs opencv4` - std = c++11
- lpigpio - lrt - lpthread
/////////////////////////////////////////////////////////////lab8
RecycleSorter::RecycleSorter(int scaleFactor = 100) :
   quit(0), state(IDLE), color(NO_MARBLE), position(RETRACTED), //global
   switchGreen(0), switchBlue(0), switchScrap(0), autoSort(0), ledGreen(0), ledBlue(0), ledRed(0),
   guiSize(5 * scaleFactor, 6 * scaleFactor), TopLeft0(0, 0), //gui
//     hueMin(0), satMin(0), valMin(0), hueMax(179), satMax(255), valMax(255), //default hsv trackers for orange (why NAT)
//     scale(20),// "scale" is a percentage
servoAngle(90)
{...}
void RecycleSorter::run() {...}
void RecycleSorter::update()
{
   while (!quit)
   {
      updateStatus();
      //printStatus(); // only to debug the status in real time
      updateOutputs();
      updateInputs();

      std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_THREAD * 100)); // this delay must be high enough to wait for the servo
   }
}
void RecycleSorter::draw() {}
void RecycleSorter::initGPIO() {}
void RecycleSorter::printStatus() {}
void RecycleSorter::updateStatus() {} // autosort still to be complete
void RecycleSorter::updateOutputs() {}
void RecycleSorter::updateInputs() {}// for later with autoSort
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
      // setup the menu
      _canvas = cv::Scalar(49, 52, 49); // clear the _canvas
      cvui::window(_canvas, 10, 10, 200, 300, WINDOW_NAME); // menu window... shrink to be as small as possible
      if (cv::waitKey(1) && (cvui::button(_canvas, 10, 30, "&Quit")))
         quit = true;

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

      cvui::update(); // ui update
      cv::imshow(WINDOW_NAME, _canvas); // Update the window where the trackbars are
   }
}
/////////////////////////////////////////////////////////////lab9
void lab9::serverthread(CServer* server) {server->start(4618);}

void lab9::do_clientserver()
{
   char inputchar = 0;
   std::vector<std::string> cmds;

   cv::VideoCapture vid;
   CServer server;

   // Start server thread
   std::thread t(&lab9::serverthread, this, &server);
   t.detach();

   vid.open(0);

   while (inputchar != 'q')
   {
      inputchar = cv::waitKey(100);

      server.get_cmd(cmds);
      if (cmds.size() > 0)
      {
         // Process different commands received by the server
         //for (size_t i = 0; i < cmds.size(); i++)
         for (int i = 0; i < cmds.size(); i++)
         {
            if (cmds.at(i) == "im")
            {
               std::cout << "\nServer Rx: " << cmds.at(i);

               std::string reply = "Hi there from Server";
               server.send_string(reply);
            }
            // add 6 if statements for get, set, manual, auto, bin1..
            // add a final IF for when to updateOutputs, & fix syntax
            else
            {
               std::cout << "\nServer Rx: " << cmds.at(i);

               std::string reply = "Got some other message";
               server.send_string(reply);
            }
         }
      }

      // Update server image with the latest camera image
      if (vid.isOpened() == true)
      {
         cv::Mat frame;
         vid >> frame;
         if (frame.empty() == false)
         {
            imshow("Server Image", frame);
            server.set_txim(frame);
         }
      }
   };
   server.stop();
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// Client side:
#include "Client.h"
std::string server_ip = "192.168.137.113";//127.0.0.1"; // raspberrypi:	192.168.137.113
int server_port = 4618;
float timeout_start;
...

int main(int argc, char* argv[])
{
   CClient client;
   int cmd = -1;

   timeout_start = cv::getTickCount();
   client.connect_socket(server_ip, server_port);
   do
   {
      print_menu();
      std::cin >> cmd;

      switch (cmd)
      {
      case 1: send_command(client, "im"); break;
      case 2: send_command(client, "cmd2"); break;
      case 3: {
         std::cout << "\nPress any key to exit\n";
         while (!_kbhit())
         {
            send_command(client, "im");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
         }
      } break;
      case 4: send_command(client, BIN1); break;
      case 5: send_command(client, BIN2); break;
      }
   } while (cmd != 0);
}
/////////////////////////////////////////////////////////////Lecs
void dilate(InputArray src, OutputArray dst, InputArray kernel, Point anchor = Point(-1, -1), int iterations = 1);
void erode(InputArray src, OutputArray dst, InputArray kernel, Point anchor = Point(-1, -1), int iterations = 1);
//src – input image; the number of channels can be arbitrary
//dst – output image of the same sizeand type as src.
//element – structuring element used for dilation; if element = Mat(), a 3 x 3 rectangular structuring element is used.
//anchor – position of the anchor within the element; default value(-1, -1) means that the anchor is at the element center.
//iterations – number of times dilation is applied.
//In general use the default values for: Kernal = Mat(), anchor = Point(-1,-1) and iterations = 1.
/**/
...
• The software for EDA(elec design automation) is KiCAD
• Where do we print these ourselves ? (tbd)
   - https ://m.pcbway.com/
• The Raspi pinout bundle to be soldered to ur PCB, then attached
• A "Via" sends the trace from top of PCB to the bottom
• A "Test Point" is a hole that can be tapped to, unlike a via, which has a protective, inductive layer
...
// Below dynamic allocation of memory will be covered, as well as the 
//limitations of the vector data type and possible alternatives using linked lists.
//To reserve a block of memory the following function is used (0 is returned if unable to reserve):
void* malloc(size_t size);
//To de-allocate the memory the following function is used:
void free(void* pointer);
//The proper C++ method for dynamic memory allocation uses the new and delete methods as follows :
CStudent * student_list;
student_list = new CStudent[2];
student_list[0]._grade = 99;
student_list[0]._id = "A00123456";
delete[] student_list;
//Never mix calls to malloc and free with calls to new and delete cuz of potential error
//A LINKED LIST is a dynamic data structure made up of nodes or elements which can point to one 
//another.The list can grow or shrink from any element of the list.The linked list uses pointers to
//point to the location in memory of subsequent elements of the list.
class CStudentLL {
public:
   float _grade;
   std::string _id;
   CStudentLL* pNext;
}; // includes pointer to the next element
CStudentLL* pList = NULL;
CStudentLL head;
head.pNext = NULL; //When the next element pointer is set to NULL, the end of the list is reached
//A function to insert an element at the front of the list might look like:
void insertfront(CStudentLL& head, CStudentLL* pnew)
{
   pnew->pNext = head.pNext;
   head.pNext = pnew;
}
// A function to remove an element from the front of the list might look like:
void removefront(CStudentLL& head)
{
   CStudentLL* toremove = head.pNext;
   if (toremove != NULL) {
      head.pNext = toremove->pNext;
      delete toremove;
   }
}
...
/*
* Concurrent Versions System (CVS) – Started 1986 (no longer updated)
* Apache Subversion (SVN) – Created to replace CVS, resolved a number of limitations
* Git, Mercurial - Distributed Version Control Systems (current standard)
* VCS Terminology:
Change - A change (or diff, or delta) represents a specific modification to a document under 
version control. 

Checkout - To check out (or co) is to create a local working copy from the repository. 

Clone - Cloning means creating a repository containing the revisions from another repository. 

Commit - To commit (check in, ci or, more rarely, install, submit or record) is to write or merge 
the changes made in the working copy back to the repository. 

Conflict - A conflict occurs when different parties make changes to the same document, and the 
system is unable to reconcile the changes.

Head - Also sometimes called tip, this refers to the most recent commit, either to the trunk or to 
a branch. 

Merge - A merge or integration is an operation in which two sets of changes are applied to a file 
or set of files. 

Pull, push - Copy revisions from one repository into another. Pull is initiated by the receiving 
repository, while push is initiated by the source. Fetch is sometimes used as a synonym for pull, 
or to mean a pull followed by an update.

Repository - The repository is where files' current and historical data are stored, often on a 
server. 

Revision - Also version: A version is any change in form. In SVK, a Revision is the state at a 
point in time of the entire tree in the repository.

Trunk - The unique line of development that is not a branch (sometimes also called Baseline, 
Mainline or Master)

Update - An update (or sync, but sync can also mean a combined push and pull) merges changes 
made in the repository (by other people, for example) into the local working copy. Update is also 
the term used by some CM tools (CM+, PLS, SMS) for the change package concept (see 
changelist). Synonymous with checkout in revision control systems that require each repository 
to have exactly one working copy (common in distributed systems)

Working copy - The working copy is the local copy of files from a repository, at a specific time 
or revision. All work done to the files in a repository is initially done on a working copy, hence 
the name. Conceptually, it is a sandbox.
*/