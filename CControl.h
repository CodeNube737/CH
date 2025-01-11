
/**
 * @file CControl.h
 * @brief This file is meant for the tm4c123gh6pm microcontroller with Firmware.ino installed via Energia.
 */

#pragma once

#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <sstream>
#include <cstdio>  // Preferred in C++
 //#include <vector>
#include "Serial.h"
#include <random>

#define JOY_X 11
#define JOY_Y 4
#define SW1 32
#define SW2 33
#define SERVO_CH 0
#define ACCEL 6
#define COMPORT 3
#define LED_R 39
#define LED_G 38
#define LED_B 37

/**
 * @enum type
 * @brief Enum to be passed into method with a type parameter.
 */
enum type
{
   DIGITAL,
   ANALOG,
   SERVO
};

/**
 * @class CControl
 * @brief This class is meant for the tm4c123gh6pm microcontroller with Firmware.ino installed via Energia.
 */
class CControl {
private:
   Serial _com; ///< Object for serial communication

public:
   /**
    * @brief Constructor.
    */
   CControl();

   /**
    * @brief Destructor.
    */
   ~CControl();

   /**
    * @brief Opens up comport, passed the comport number required.
    * @param comport The comport number.
    */
   void init_com(int comport);

   /**
    * @brief Searches 20 comports if the serial connection was lost during transmission.
    * @param comport The comport number.
    * @return True if the port is found, false otherwise.
    */
   bool portSearch(int comport);

   /**
    * @brief Writes to u-controller to send an acknowledge result for a certain channel, then reads that value and returns true.
    * @param type The type of the data.
    * @param channel The channel number.
    * @param result The result value.
    * @return True if the operation is successful, false otherwise.
    */
   bool get_data(int type, int channel, int& result);

   /**
    * @brief Writes to u-controller to set result for a certain channel.
    * @param type The type of the data.
    * @param channel The channel number.
    * @param val The value to set.
    * @return True if the operation is successful, false otherwise.
    */
   bool set_data(int type, int channel, int val);

   /**
    * @brief Calls get_data and returns a percentage of the total x or y joystick position.
    * @param type The type of the data.
    * @param channel The channel number.
    * @param result The result value.
    * @return The percentage of the total x or y joystick position.
    */
   float get_analog(int type, int channel, int& result);

   /**
    * @brief Works just like get_analog, but it doesn't require get analog. It also is passed the point class.
    * @param type The type of the data.
    * @param result The result value.
    * @return The percentage of the total x or y joystick position.
    */
   float get_joystick(int type, cv::Point& result);

   /**
    * @brief Calls get_data and returns true based on digital pushbutton. It also debounces the pushbutton.
    * @param type The type of the data.
    * @param channel The channel number.
    * @param result The result value.
    * @return True if the button is pressed, false otherwise.
    */
   bool get_button(int type, int channel, int& result);

   /**
    * @brief Print menu for user to select desired test.
    */
   void lab3_print_menu();

   /**
    * @brief Run test for joystick.
    */
   void analogTest();

   /**
    * @brief Run test for a digital pushbutton.
    */
   void digitalTest();

   /**
    * @brief Run test for a debounced digital pushbutton.
    */
   void debounceTest();

   /**
    * @brief Sweep the entire range of an sg90.
    * @return The result of the operation.
    */
   int servoTest();

   /**
    * @brief A test function made to see how the serial communication works.
    * @param send The string to send.
    */
   void comTest(std::string send);

   int channelx = 11; ///< The x channel number.
   int channely = 4; ///< The y channel number.
   int channel_debounce = 32; ///< The debounce channel number.
   int channel_digital = 33; ///< The digital channel number.
   int channel_servo = 0; ///< The servo channel number.
   int channel_accelerometer = 6; ///< The accelerometer channel number.
   int comPort = 3; ///< The comport number.
};
