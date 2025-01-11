////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated Dec 2, 2022
////////////////////////////////////////////////////////////////

#include "stdafx.h"

// Add simple GUI elements
#include "cvui.h" // lab4 onward

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include "Client.h"
#include "Server.h"

// Must include Windows.h after Winsock2.h, so Serial must be included after Client/Server
#include "Serial.h" 
#include "myTrial.h"

#define CANVAS_NAME "Display Image"

////////////////////////////////////////////////////////////////
// Can be used as a replacement for cv::waitKey() to display cv::imshow() images, Windows Only
////////////////////////////////////////////////////////////////
void process_msg()
{
   MSG msg;
   while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
   {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
   }
}

////////////////////////////////////////////////////////////////
// Generate ARUCO markers
////////////////////////////////////////////////////////////////
void generate_marks()
{
   std::string str;
   cv::Mat im;
   int mark_size = 250;

   cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

   for (int marker_id = 0; marker_id < 250; marker_id++)
   {
      cv::aruco::drawMarker(dictionary, marker_id, mark_size, im, 1);
      str = "mark" + std::to_string(marker_id) + ".png";
      cv::imwrite(str, im);
   }
}

////////////////////////////////////////////////////////////////
// Serial Communication
////////////////////////////////////////////////////////////////
void test_com()
{
   // Comport class (change port to your MSP device port)
   Serial com;
   com.open("COM4"); /// this is important... COM4 or COM3?

   // TX and RX strings
   std::string tx_str = "G 1 11\n";
   std::string rx_str;

   // temporary storage
   char buff[2];
   do
   {
      // Send TX string
      com.write(tx_str.c_str(), tx_str.length());
      Sleep(10); // wait for ADC conversion, etc. May not be needed?

      rx_str = "";
      // start timeout count
      double start_time = cv::getTickCount();

      buff[0] = 0;
      // Read 1 byte and if an End Of Line then exit loop
      // Timeout after 1 second, if debugging step by step this will cause you to exit the loop
      while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
      {
         DWORD bytes_read;
         com.read(buff, 1); // , &bytes_read), acording to FstYr_code
         if (bytes_read > 0)
         {
            rx_str = rx_str + buff[0];
         }
      }

      printf("\nRX: %s", rx_str.c_str());
      cv::waitKey(1);
   } while (1);
}

////////////////////////////////////////////////////////////////
// Display Image on screen
////////////////////////////////////////////////////////////////
void do_image()
{
   cv::Mat im;
   bool quit = false;

   // initialize GUI system
   cvui::init(CANVAS_NAME);
   cv::Point gui_position;

   // Load test image
   im = cv::imread("BCIT.jpg");

   // Seed random number generator with time
   srand(time(0));

   while (!quit)
   {
      // Draw 500 circles and dots on the image
      for (int i = 0; i < 500; i++)
      {
         gui_position = cv::Point(10, 10);
         cvui::window(im, gui_position.x, gui_position.y, 200, 40, "Image Test");
         gui_position += cv::Point(5, 25);
         cvui::text(im, gui_position.x, gui_position.y, "Number of Circles: " + std::to_string(i));

         float radius = 50 * rand() / RAND_MAX;
         cv::Point center = cv::Point(im.size().width * rand() / RAND_MAX, im.size().height * rand() / RAND_MAX);

         cv::circle(im, center, radius, cv::Scalar(200, 200, 200), 1, cv::LINE_AA);

         im.at<char>(i, i) = 255;

         // Update the CVUI GUI system
         cvui::update();

         // Show image and delay (all display image to update)
         cv::imshow(CANVAS_NAME, im);
         //cv::waitKey(1);

         if ( i == (500 - 1) )
            quit = true;
      }
   }
}

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
   cv::VideoCapture vid;

   vid.open(0, cv::CAP_DSHOW);

   bool do_canny = true;
   bool do_aruco = false;
   int canny_thresh = 30;
   bool do_exit = false;

   cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

   // initialize GUI system
   cvui::init(CANVAS_NAME);
   cv::Point gui_position;

   std::vector<cv::Scalar> color_vec;
   color_vec.push_back(cv::Scalar(255, 255, 255));
   color_vec.push_back(cv::Scalar(255, 0, 0));
   color_vec.push_back(cv::Scalar(0, 255, 0));
   color_vec.push_back(cv::Scalar(0, 0, 255));
   int color_index = 0;

   if (vid.isOpened() == TRUE)
   {
      do
      {
         cv::Mat frame, edges;

         // Capture video frame
         vid >> frame;

         // Make sure video frame exists
         if (frame.empty() == false)
         {
            // ARUCO marker tracking
            if (do_aruco == true)
            {
               std::vector<int> ids;
               std::vector<std::vector<cv::Point2f> > corners;
               cv::aruco::detectMarkers(frame, dictionary, corners, ids);
               if (ids.size() > 0)
               {
                  cv::aruco::drawDetectedMarkers(frame, corners, ids);
               }
            }

            // Canny edge detection
            if (do_canny == true)
            {
               cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
               cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
               cv::Canny(edges, edges, 0, canny_thresh, 3);
               cv::add(frame, color_vec.at(color_index), frame, edges);
            }

            // GUI Menu
            gui_position = cv::Point(10, 10);
            cvui::window(frame, gui_position.x, gui_position.y, 200, 190, "Video Test");
            gui_position += cv::Point(5, 25);
            cvui::checkbox(frame, gui_position.x, gui_position.y, "Canny Filter", &do_canny);
            gui_position += cv::Point(0, 25);
            cvui::checkbox(frame, gui_position.x, gui_position.y, "ArUco", &do_aruco);
            gui_position += cv::Point(0, 25);
            cvui::text(frame, gui_position.x, gui_position.y, "Canny Threshold");
            gui_position += cv::Point(0, 15);
            cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &canny_thresh, 5, 120);
            gui_position += cv::Point(0, 50);
            if (cvui::button(frame, gui_position.x, gui_position.y, 100, 30, "Colour Switch"))
            {
               color_index++;
               if (color_index >= color_vec.size()) { color_index = 0; }
            }
            gui_position += cv::Point(120, 0);
            if (cvui::button(frame, gui_position.x, gui_position.y, 50, 30, "Exit"))
            {
               do_exit = true;
            }

            // Update the CVUI GUI system
            cvui::update();
            cv::imshow(CANVAS_NAME, frame);
         }
      } while (cv::waitKey(1) != 'q' && do_exit == false);
   }
}

////////////////////////////////////////////////////////////////
// Demo client server communication
////////////////////////////////////////////////////////////////
void serverthread(CServer* server)
{
   // Start server
   server->start(4618);
}

void do_clientserver()
{
   char inputchar = 0;
   std::vector<std::string> cmds;

   cv::VideoCapture vid;
   CServer server;

   // Start server thread
   std::thread t(&serverthread, &server);
   t.detach();

   vid.open(0);

   while (inputchar != 'q')
   {
      inputchar = cv::waitKey(100);

      server.get_cmd(cmds);
      if (cmds.size() > 0)
      {
         // Process different commands received by the server
         for (int i = 0; i < cmds.size(); i++)
         {
            if (cmds.at(i) == "im")
            {
               std::cout << "\nServer Rx: " << cmds.at(i);

               std::string reply = "Hi there from Server";
               server.send_string(reply);
            }
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
            process_msg();
            server.set_txim(frame);
         }
      }
   };

   server.stop();

   Sleep(100);
}

////////////////////////////////////////////////////////////////
// Lab 1
// This is supposed to be more poorly done, using the C onventions u have learned thus far
// the next lab will be the better version of this 
////////////////////////////////////////////////////////////////////////////////////////
#include "lab1.h"

// function prints out the menu to instruct the instructor
void lab1_print_menu()
{
   std::cout << std::endl;
   std::cout << "***************************************\n";
   std::cout << "ELEX 4818 GRADE SYSTEM, by Mikhail Rego\n";
   std::cout << "***************************************\n";
   std::cout << "(A)dd student\n";
   std::cout << "(E)dit student \n";
   std::cout << "(P)rint grades \n";
   std::cout << "(Q)uit \n\n";
}

// Function to add a student
void lab1_add_student() {
   if (number_of_students < MAX_STUDENTS) {
      // Increment the number_of_students
      number_of_students++;

      // Call the edit_student function for the newly added student
      lab1_edit_student(number_of_students - 1);  // Pass the index of the newly added student
   }
   else {
      std::cout << "Maximum number of students reached." << std::endl;
   }
}

// Function to edit a student
void lab1_edit_student(int EditIndex) {
   std::cout << "Enter new student name: ";
   std::cin >> students[EditIndex].StudentNumber1;

   std::cout << "Enter new lab grade: ";
   std::cin >> students[EditIndex].lab1;

   std::cout << "Enter new quiz grade: ";
   std::cin >> students[EditIndex].quiz1;

   std::cout << "Enter new midterm grade: ";
   std::cin >> students[EditIndex].MT1;

   std::cout << "Enter new final grade: ";
   std::cin >> students[EditIndex].final1;

   students[EditIndex].total1 =
      lab_weight * students[EditIndex].lab1 +
      quiz_weight * students[EditIndex].quiz1 +
      MT_weight * students[EditIndex].MT1 +
      final_weight * students[EditIndex].final1;

   std::cout << "Student information updated." << std::endl;
}

// Function to print all grades
void lab1_print_grades()
{
   std::cout << "CMD > P \n\n";
   std::cout << "#    Student    Lab    Quiz    Midterm    Final Exam    Final Grade \n";
   for (int i = 0; i < number_of_students; i++) {
      std::cout << students[i].index << "    ";
      std::cout << students[i].StudentNumber1 << "  ";
      std::cout << students[i].lab1 << "   ";
      std::cout << students[i].quiz1 << "    ";
      std::cout << students[i].MT1 << "       ";
      std::cout << students[i].final1 << "          ";
      std::cout << students[i].total1 << std::endl;
   }
}

char quit(void)
{
   return 'Q'; // this should trigger an escape sequence in the main or lab1 function
}
//////////////////////////////////////////////////////////////////////////////////////////
void lab1()
{
   while (input != ('Q'))
   {
      lab1_print_menu();
      std::cin >> input;
      input = toupper(input);

      switch (input)
      {
      case('A'):
         lab1_add_student();
         break;


      case 'E':
         int EditIndex;
         std::cout << "Enter the student index to edit: ";
         std::cin >> EditIndex;
         if (EditIndex >= 1 && EditIndex <= number_of_students) {
            lab1_edit_student(EditIndex - 1);
         }
         else {
            std::cout << "Invalid student index." << std::endl;
         }
         break;


      case('P'):
         lab1_print_grades();
         break;
      case 'Q':
         input = quit();
         break;
      default:
         std::cout << '\n' << "that wasn't an option" << '\n';
      }
   }
}

////////////////////////////////////////////////////////////////
// lab2.cpp - grading
// by: 	Mikhail R	01/27/2024
////////////////////////////////////////////////////////////////
#include "lab2.h" 
using namespace std; // makes the main file look neater

// this should also be in the .h-file and the CCourse file, but wtvr
void lab2_print_menu()
{
   cout << "***************************************\n";
   cout << "ELEX 4618 GRADE SYSTEM, by MIKHAIL REGO\n";
   cout << "***************************************\n";
   cout << "(A)dd student\n";
   cout << "(E)dit student \n";
   cout << "(D)elete student \n";
   cout << "(P)rint grades \n";
   cout << "(Q)uit \n";
   cout << "(L)oad \n";
   cout << "(S)ave \n";
   //cout << "CMD: "; /// remove if using template
}
int lab2()
{
   CCourse MyCourse;
   char ans = NULL;
   while (ans != 'Q') {
      lab2_print_menu();
      cin >> ans;
      ans = toupper(ans);
      switch (ans) {
      case'A':
         MyCourse.add_student();
         break;
      case'E':
         MyCourse.edit_student();
         break;
      case'D':
         MyCourse.delete_student();
         break;
      case'P':
         MyCourse.print_grades();
         break;
      case'L':
         MyCourse.load_class();
         break;
      case'S':
         MyCourse.save_class();
         break;
      }
   }
   return 0; // i tried ghangin lab2() to an int instead of a void... but i'm still getting the buffer output... let's debug
}

////////////////////////////////////////////////////////////////
// Lab 3
////////////////////////////////////////////////////////////////
#include "CControl.h"

void lab3()
{
   
   CControl ControlTest; // instantiation of CControl class into main (here called lab3) function
   myTrial trial3;

   char input3 = NULL;

   ControlTest.init_com(3); // commented-out cuz this needs to be included in every function

   while (input3 != 'Q') 
   {
      ControlTest.lab3_print_menu();
      std::cin >> input3;
      input3 = toupper(input3);

      switch (input3)
      {
      case 'A': ControlTest.analogTest(); break;
      case 'B': ControlTest.debounceTest(); break;
      case 'D': ControlTest.digitalTest(); break;
      case 'S': ControlTest.servoTest(); break;

      // the next few are tests, not included in menu
      //case 'C': ControlTest.comTest("S 2 0 180"); break;
      //case 'T': trial3.TestCom("G 0 0"); break;
      case 'P': ControlTest.portSearch(3); break;

      default: std::cout << "Invalid input\n"; break;
      }

   }
   
}

////////////////////////////////////////////////////////////////
// Lab 4
////////////////////////////////////////////////////////////////
//#include "CControl.h" // commented out redundancy
#include "CBase4618.h"
#include "CSketch.h"

void lab4()
{
   myTrial trial4; // don't comment this out

   //... this should only have the run function
   CSketch EtchAsketch(750);
   EtchAsketch.run();

   //int x = 1; // for the myPoint function.
   //int y = 2; // if passing the point vector to other functions, u need variables x & y declared.
   //trial4.myPoint(x,y);  // this will be commented out later
   //trial4.T_draw();      // this will be commented out later
   //trial4.Test_joy();    // this will be commented out later
}

////////////////////////////////////////////////////////////////
// Lab 5
////////////////////////////////////////////////////////////////
#include "CPong.h"
void lab5()
{
   myTrial trial5;
   CPong Pong(90);
   Pong.run();

   //trial5.LedToggle();    // this will be commented out later
   //trial5.T_draw2();      // this will be commented out later
   //trial5.Ballon();       // this will be commented out later
   //trial5.myPoint(Pong.PonglordScore, Pong.playerScore); // this will be commented out later
}

////////////////////////////////////////////////////////////////
// Lab 6
////////////////////////////////////////////////////////////////
#include "CAsteroidGame.h"
void lab6()
{
   CAsteroidGame Asteroids(90);
   Asteroids.run();
}

////////////////////////////////////////////////////////////////
// Lab8 Trial
////////////////////////////////////////////////////////////////
#include "Test8.h"
#include "Test8A.h"
#include "TestVideo.h"
void lab8T()
{
   Test8 Test_8;
   Test8A Test8A(90);
   TestVideo TestVideo;

   //Test_8.run(); // assuming Test8 has a run method
   //Test8A.run();
   TestVideo.do_video();
}


////////////////////////////////////////////////////////////////
// led toggle
////////////////////////////////////////////////////////////////
void leds()
{
   myTrial led;
   led.LedToggle();
}

void print_menu()
{
   std::cout << "\n***********************************";
   std::cout << "\n* ELEX4618 Template Project";
   std::cout << "\n***********************************";
   std::cout << "\n(1) Lab 1 - User Input";
   std::cout << "\n(2) Lab 2 - Grading";
   std::cout << "\n(3) Lab 3 - Embedded Control";
   std::cout << "\n(4) Lab 4 - Etch-A-Sketch";
   std::cout << "\n(5) Lab 5 - Pong";
   std::cout << "\n(6) Lab 6 - Classic Arcade Game";
   std::cout << "\n(7) Lab 7 - Linux Port";
   std::cout << "\n(8) Lab 8 - Sorting";
   std::cout << "\n(9) Lab 9 - Sockets";
   std::cout << "\n(10) Test serial COM communication";
   std::cout << "\n(11) Show image manipulation";
   std::cout << "\n(12) Show video manipulation";
   std::cout << "\n(13) Test client/server communication";
   std::cout << "\n(l) LED Toggle";
   std::cout << "\n(0) Exit";
   std::cout << "\nCMD> ";
}

int main(int argc, char* argv[])
{
   // generate_marks();

   int cmd = -1;
   do
   {
      print_menu();
      std::cin >> cmd;
      switch (cmd)
      {
      case 1: lab1(); break;
      case 2: lab2(); break;
      case 3: lab3(); break;
      case 4: lab4(); break;
      case 5: lab5(); break;
      case 6: lab6(); break;
      case 8: lab8T(); break;
      case 'l': leds(); break;
      case 10: test_com(); break;
      case 11: do_image(); break;
      case 12: do_video(); break;
      case 13: do_clientserver(); break;
      }
   } while (cmd != 0);
}
