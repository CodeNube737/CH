//CControl.cpp
// defines CControl and all control functions... i think.
// The CControl class will function as the C++ object used for communication with the embedded system

#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "CControl.h"

CControl::CControl() {}
CControl::~CControl() {}

void CControl::init_com(int comport) 
{
   _com.flush();
   std::string Comport = "COM" + std::to_string(comport);

   if (!_com.open(Comport))
      std::cerr << "Failed to open" << Comport << std::endl;
}

bool CControl::portSearch(int comport)
{
   do
   {
      for (int i = 0; i < 20; i++)
      {
         init_com(i);
         if (_com.is_open()) { return true; }
         //std::cout << "No connection at COM" << i << '\n';
      }
   } while (true);
}

bool CControl::get_data(int type, int channel, int& result) {
   // Buffer to hold the response
   char buffer[256];

   // Format the data to be sent
   sprintf(buffer, "G %d %d\n", type, channel);

   // Write the buffer to the COM port
   _com.write(buffer, strlen(buffer));

   std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Wait for 1 millisecond

   // Read the response
   int bytesRead = _com.read(buffer, sizeof(buffer) - 1);

   // Null-terminate the string
   buffer[bytesRead] = '\0';

   // Parse the received data
   std::stringstream ss(buffer);
   std::string temp;
   int parsedResult = 0;

   // The following while-loop gets the 'result' portion of the buffer, since the type and channel shouldn't change
   while (ss >> temp)
   {
      if (std::stringstream(temp) >> parsedResult) {
         // Successfully parsed an integer, update result
         result = parsedResult;
      }
   }

   // Print the response (un-comment if you want to see what's actually being received)
   //std::cout << "Received: " << buffer << "; result to use: " << result << std::endl;

   // If result was updated, the data was successfully read
   if (result != parsedResult)
      return false;

   // If we reach this point, the data was successfully read
   return true;
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

float CControl::get_analog(int type, int channel, int& result)
{
   if ( get_data(ANALOG, channel, result) ) // Call the get_data function to retrieve analog input
   {
      // Assuming a 12-bit ADC, calculate percentage of full scale
      float percentage = static_cast<float>(result) / 4096.0f * 100.0f;
      return percentage;
   }
   else 
      portSearch(3);
}

// get_joystick is an updated get analog, that does NOT call get_data, and passes result as a cv::Point(x,y)
float CControl::get_joystick(int type, cv::Point& result)
{
   // Buffer to hold the response
   char buffer[256];

   // Channels to read from
   int channels[2] = { 11, 4 };

   for (int i = 0; i < 2; i++) {
      // Format the data to be sent
      sprintf(buffer, "G %d %d\n", type, channels[i]);

      // Write the buffer to the COM port
      _com.write(buffer, strlen(buffer));

      std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Wait for 1 millisecond

      // Read the response
      int bytesRead = _com.read(buffer, sizeof(buffer) - 1);

      // Null-terminate the string
      buffer[bytesRead] = '\0';

      // Parse the received data
      std::stringstream ss(buffer);
      std::string temp;
      int parsedType, parsedChannel, parsedResult;

      // The following while-loop gets the 'result' portion of the buffer, since the type and channel shouldn't change
      ss >> temp; // Read the 'A'
      if (ss >> parsedType >> parsedChannel >> parsedResult) {
         // Successfully parsed the integers, update result
         if (i == 0) {
            result.x = parsedResult;
         }
         else {
            result.y = parsedResult;
         }
      }
      //std::cout << result.x << ", " << result.y << std::endl;
   }

   // Assuming a 12-bit ADC, calculate percentage of full scale
   float percentage = static_cast<float>(result.x) / 4096.0f * 100.0f;

   // If we reach this point, the data was successfully read
   return percentage;
}

bool CControl::get_button(int type, int channel, int& result) // debouncing method
{
   if (get_data(ANALOG, channel, result))
   {
      static auto lastPressTime = std::chrono::steady_clock::now(); // Time of the last button press
      static bool buttonState = false; // State of the button (true = pressed, false = released)
      auto now = std::chrono::steady_clock::now(); // Current time

      if (this->get_data(DIGITAL, channel, result))
      {
         if (result == 0 && !buttonState && std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPressTime).count() > 38)
         {
            // Button is pressed, was previously released, and enough time has passed since the last press
            lastPressTime = now; // Update the time of the last button press
            buttonState = true; // Update the button state
            return true;
         }
         else if (result != 0)
         {
            // Button is released
            buttonState = false; // Update the button state
         }
      }
      return false;
   }
   else portSearch(3);
}

void CControl::lab3_print_menu()
{
   std::cout << "******************************************\n";
   std::cout << "ELEX 4618 Test Controller, by MIKHAIL REGO\n";
   std::cout << "******************************************\n";
   std::cout << "(A)nalog test\n";
   std::cout << "(D)igital button test\n";
   std::cout << "de(B)ounce button test\n";
   std::cout << "(S)ervo motor test\n";
   std::cout << "(Q)uit menu\n";
}

void CControl::digitalTest()
{
   // Initialize the communication port... it seems this needs to be done in the indiv functions every time!
   //this->init_com(3);

   int channel = 33; // BP_S1
   int result;

   while(!_kbhit()) // Infinite loop
   {
      if (get_data(DIGITAL, channel, result)) { // Read from digital channel 33
         std::cout << "DIGITAL TEST: " << channel << ' ' << result << '\n'; // Print the result before inverting it
         int led = !result; // Invert the result for led
         set_data(DIGITAL, 38, led); // Write the inverted result to channel 38
      }
      else portSearch(3);  //std::cout << "Failed to get data, press \"Ctrl + c\"\n";
      // Add a delay here if necessary, for example:
      //std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }
}

int CControl::servoTest() { 
  
   // Create an instance of CControl
   //CControl control;

   // Initialize the communication port... it seems this needs to be done in the indiv functions every time!
   //control.init_com(3);

   // Set the data to be sent
   int channel = 0;
   int val = 92;

   // Send the data
   bool success = set_data(SERVO, channel, val);

   // Check if the data was sent successfully
   if (success)
      std::cout << "Data sent successfully.\n";
   else 
      std::cout << "Failed to send data.\n";
   std::cout << "SERVO TEST: CH" << channel << " POSITION " << val << "°\n";
   std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second
   
   //////////////////////////////////////////////////////////////////////////
   val = 180;
   set_data(SERVO, channel, val);
   std::cout << "SERVO TEST: CH" << channel << " POSITION " << val << "°\n";
   std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second

   //////////////////////////////////////////////////////////////////////////
   val = 1;
   set_data(SERVO, channel, val);
   std::cout << "SERVO TEST: CH" << channel << " POSITION " << val << "°\n";
   std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second

   //////////////////////////////////////////////////////////////////////////
   val = 180;
   set_data(SERVO, channel, val);
   std::cout << "SERVO TEST: CH" << channel << " POSITION " << val << "°\n";
   std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second

   //////////////////////////////////////////////////////////////////////////
   val = 92;
   set_data(SERVO, channel, val);
   std::cout << "SERVO TEST: CH" << channel << " POSITION " << val << "°\n";
   std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second

   return 0;
}

void CControl::debounceTest() 
{
   // Initialize the communication port... it seems this needs to be done in the indiv functions every time!
   //this->init_com(3);

   int result;
   int channel = 32;
   int count = 0;

   while (!_kbhit()) // Infinite loop
   {
      if(this->get_button(DIGITAL, channel, result))  // Read from digital channel 32... but should use get_button instead
         if (!result)
            std::cout << "BUTTON TEST: " << ++count << '\n';
   }
   //else std::cout << "Failed to get data, press \"Ctrl + c\"\n";
   // Add a delay here if necessary, for example:
   //std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void CControl::analogTest() // this function needs to sit in a ****while(!kbhit) loop that constantly passes it 5 int's
{

   // Initialize the communication port... it seems this needs to be done in the indiv functions every time!
   //this->init_com(3);

   int channelx = 11;
   int resultx;
   int channely = 4;
   int resulty;

   while (!_kbhit()) // Infinite loop
   {
      float PercentX = get_analog(ANALOG, channelx, resultx); // hopefully doesn't have too many decimals... that'd look ugly printed
      float PercentY = get_analog(ANALOG, channely, resulty); // get_analog is a percentage finder
      set_data( SERVO, 0, (PercentX*180/100) );
      std::cout << "ANALOG TEST: " << channelx << "(X) = " << resultx << ", " << PercentX << "% " << channely << "(Y) = " << resulty << ", " << PercentY << "% " << '\n';
   }     
} // note, i think return values, etc. will be a nightmare in this thing without the point class! so do that!

void CControl::comTest(std::string send)
{
   // Create an instance of CControl
   CControl control;

   // Initialize the communication port... it seems this needs to be done in the indiv functions every time!
   //control.init_com(3);

   int type = SERVO, channel = 0, result;

   control.get_data(type, channel, result);

   std::cout << "A " << type << ' ' << channel << ' ' << result << '\n';
}
