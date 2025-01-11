//MT2 ftsht
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
   std::vector<CStudent> Course1;
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
         //std::cout << "No connection at COM" << i << '\n';
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
   while (!quit)
   {
      std::thread drawThread(&CPong::draw, this);
      std::thread updateThread(&CPong::update, this);
      drawThread.join();
      updateThread.join();
      quit = waitKey(1);
   } exit(0);
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