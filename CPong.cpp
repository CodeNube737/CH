#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "cvui.h" // for lab4 onward. To be included in .cpp file, not .h file
#include "CBase4618.h"
#include "CPong.h" 

CPong::CPong(int scaleFactor = 100) :
   //global
   reset(0), quit(0), count(0), rand1(1), rand2(1),// win(false),
   //gui
   guiSize(10 * scaleFactor, 8 * scaleFactor), TopLeft0(0, 0),
   //default
   DefaultBall(guiSize.x / 2, guiSize.y / 2), DefaultPlayer(guiSize.x - scaleFactor / 10, guiSize.y / 2), DefaultPonglord(TopLeft0.x + scaleFactor / 10, guiSize.y / 2),
   ballSpeed(10), ballDirection(1,1), oldPosition(guiSize.x / 2, guiSize.y / 2), newPosition(0,0), t(1), 
   //ball
   ballRadius(40), ballCentre(guiSize.x / 2, guiSize.y / 2),
   //paddles
   paddleSize(scaleFactor / 5, scaleFactor * 1.5), playerCenter(guiSize.x - scaleFactor / 10, guiSize.y / 2), PonglordCenter(TopLeft0.x + scaleFactor / 10, guiSize.y / 2),
   playerScore(0), PonglordScore(0), winningScore(5), Ponglordskill(1),
   //borders
   ScrnTop(TopLeft0.y), ScrnRight(guiSize.x), ScrnBott(guiSize.y), ScrnLeft(TopLeft0.x)
{
// voici ou l'initialization veut etre, mais on ne sait pas pourquoi apres la compilation et court, il dit <<avortez>>..?
}

void CPong::run()
{
   using namespace cv;
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
            /////////////////////////////////////////////////////////////////////////////////
            // final sweep final toc of chrono stamp
            //std::this_thread::sleep_for(std::chrono::milliseconds(int(fpsCounter.delay))); // this is if u were using the FPS trackbar in straed of for ballSpeed
            //fpsCounter.update(); // Update FPS counter
         }
         else // skips updates and resets to initial conditions
         {
            rand1 = randomServe();
            rand2 = randomServe();
            // reset to default
            ballDirection = cv::Point2f(rand1, rand2); oldPosition = DefaultBall; ballCentre = DefaultBall; //ball
            playerCenter = DefaultPlayer; PonglordCenter = DefaultPonglord; //paddles
            oldPosition = playerCenter - ballDirection;
            // now proceed as usual
            updateBall(ballCentre);
            updatePaddles(playerCenter.y, PonglordCenter.y);
            std::cout << "New Game!\n";
         }
      }
      //velocity(ballCentre); // for test... delete when code works
      //borderLine(playerCenter.y, PonglordCenter.y); // for test... delete when code works

      if (reset)
      {
         if ((playerScore == 5))// win??
         {
            std::cout << "winner!\n";
         }
         else 
         {
            std::cout << "loser!\n";
         }
         //win = false;
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
      }
        
      
      // draw the gui & its elements based on constructor's variables
      cvui::rect(_canvas, playerCenter.x - paddleSize.x, playerCenter.y - paddleSize.y/2, paddleSize.x, paddleSize.y, 0xffffff, 0xffffff); // '_canvas', x, y, width, height, border color, fill color
      cvui::rect(_canvas, PonglordCenter.x, PonglordCenter.y - paddleSize.y / 2, paddleSize.x, paddleSize.y, 0xffffff, 0xffffff); // '_canvas', x, y, width, height, border color, fill color
      cv::circle(_canvas, ballCentre, ballRadius, cv::Scalar(0xff, 0xff, 0xff), CVUI_FILLED); // (i/o-array, centre(Pt), radius, color(Scalar), thickness or filled, lineType, shift)
      cvui::printf(_canvas, 25, 70, "Score");
      cvui::printf(_canvas, 25, 85, "Player: %d", playerScore);
      cvui::printf(_canvas, 25, 100, "Ponglord: %d", PonglordScore);
      cvui::printf(_canvas, 25, 110 + 25, "Size");
      cvui::trackbar(_canvas, 0 + 50, 110, 200 - 50, &ballRadius, (double)5.0, (double)100.0);
      cvui::printf(_canvas, 25, 160, "Frames/sec: %f", fpsCounter.FPS);
      //cvui::trackbar(_canvas, 0 + 15, 170, 200 - 15, &fpsCounter.delay, (double)0.0, (double)50.0); // is supposed to be ballSpeed, not fpsDelay (comment this out, don't change it)
      cvui::printf(_canvas, 25, 170 + 25, "Speed");
      cvui::trackbar(_canvas, 0 + 50, 170, 200 - 50, &ballSpeed, (int)1, (int)35);  // if the ball speed gets too high, my side border conditions break-down... so limit @30
      cvui::printf(_canvas, 000 + 25, 230 + 60, "Easy");
      cvui::printf(_canvas, 200 - 25, 230 + 60, "Hard");
      cvui::trackbar(_canvas, 0 + 25, 230, 200 - 25, &Ponglordskill, (float)1, (float)6.66);
      cvui::update();
      cv::imshow(WINDOW_NAME, _canvas);
   }
}
// borderLine was more of a trial function... u may not need it
void CPong::updatePaddles(float& playerCenter_y, float& PonglordCenter_y)
{
   int get;// just cuz i made an akward get setup
   if ((int)control.get_analog(ANALOG, JOY_Y, get) > 60)
      playerCenter_y -= ballSpeed;

   if ((int)control.get_analog(ANALOG, JOY_Y, get) <= 40)
      playerCenter_y += ballSpeed;

   PonglordCenter_y -= ballSpeed * (PonglordCenter_y - ballCentre.y) / (PonglordCenter_y + ballCentre.y) * Ponglordskill;

   ballTopLeft = ballCentre + cv::Point2f(-1 * ballRadius, -1 * ballRadius);
   ballBottLeft = ballCentre + cv::Point2f(-1 * ballRadius, 1 * ballRadius);
   ballTopRight = ballCentre + cv::Point2f(1 * ballRadius, -1 * ballRadius);
   ballBottRight = ballCentre + cv::Point2f(1 * ballRadius, 1 * ballRadius);

   playerTopLeft = playerCenter + cv::Point2f(paddleSize.x / -2, paddleSize.y / -2);
   playerBottLeft = playerCenter + cv::Point2f(paddleSize.x / -2, paddleSize.y / 2);
   PonglordTopRight = PonglordCenter + cv::Point2f(paddleSize.x / 2, paddleSize.y / -2);
   PonglordBottRight = PonglordCenter + cv::Point2f(paddleSize.x / 2, paddleSize.y / 2);

   if (playerBottLeft.y > ScrnBott)    
      playerCenter_y = ScrnBott - paddleSize.y/2;
   else if (playerTopLeft.y < ScrnTop)           
      playerCenter_y = ScrnTop + paddleSize.y / 2;

   if (PonglordBottRight.y > ScrnBott)
      PonglordCenter_y = ScrnBott - paddleSize.y / 2;
   else if (PonglordTopRight.y < ScrnTop)
      PonglordCenter_y = ScrnTop + paddleSize.y / 2;
   /*
   else
   {
      playerCenter_y = playerheight;
      PonglordCenter_y = Ponglordheight;
   }
   */
   // any more code...? 
   // ... all of this should be in void update, i think...
}
/*
// note that ball.y shouldn't be bothered, cuz it should be in the "velocity" method
    if (ball.y + radius > guiSize.y) // says if ball is at bot border    
       ball.y = ... velocity
    if (ball.y + radius < 0) // says if ball is at top border    
        
// note that ball.x shouldn't be bothered, cuz it should be in the "winning" method
    if (playerPosition - paddleSize.y/2 < 0) // says if player is at top border             
        
    if (playerPosition - paddleSize.y/2 < 0) // says if player is at top border            
       
*/

void CPong::updateBall(cv::Point2f& ballPosition)
{
   ballTopLeft = ballCentre + cv::Point2f(-1 * ballRadius, -1 * ballRadius);
   ballBottLeft = ballCentre + cv::Point2f(-1 * ballRadius, 1 * ballRadius);
   ballTopRight = ballCentre + cv::Point2f(1 * ballRadius, -1 * ballRadius);
   ballBottRight = ballCentre + cv::Point2f(1 * ballRadius, 1 * ballRadius);

   playerTopLeft = playerCenter + cv::Point2f(paddleSize.x / -2, paddleSize.y / -2);
   playerBottLeft = playerCenter + cv::Point2f(paddleSize.x / -2, paddleSize.y / 2);
   PonglordTopRight = PonglordCenter + cv::Point2f(paddleSize.x / 2, paddleSize.y / -2);
   PonglordBottRight = PonglordCenter + cv::Point2f(paddleSize.x / 2, paddleSize.y / 2);

    ballvelocity = ballSpeed * ballDirection;
   //t = (float)fpsCounter.FPS/10; // FPS always seems to throw things off... clock cycles are running regardless of whether u notice them.
   newPosition = ballPosition + ballvelocity* t;

   ballDirection = ( (newPosition - oldPosition) /
      sqrt( (newPosition.x - oldPosition.x) * (newPosition.x - oldPosition.x)
         + (newPosition.y - oldPosition.y) * (newPosition.y - oldPosition.y) ) ); // v^ = s^ , [except for units] cuz it's direction
   oldPosition = ballPosition; // should be initialized in constructor to the ball's default position
   ballPosition = newPosition;

   ballTopLeft = ballCentre + cv::Point2f(-1 * ballRadius, -1 * ballRadius);
   ballBottLeft = ballCentre + cv::Point2f(-1 * ballRadius, 1 * ballRadius);
   ballTopRight = ballCentre + cv::Point2f(1 * ballRadius, -1 * ballRadius);
   ballBottRight = ballCentre + cv::Point2f(1 * ballRadius, 1 * ballRadius);

   /////////////////////////////////////////////////
   // border conditions of ball, based on velocity
   // do this AFTER u get the ball moving (STEP-BY-STEP!)
   if ( (ballBottRight.x - paddleSize.x/2 >= playerBottLeft.x) & ((ballTopRight.y <= playerBottLeft.y) & (ballBottRight.y >= playerTopLeft.y)) )
   {
      oldPosition = newPosition + -1 * ballvelocity; //+ ballDirection * ballvelocity.x;// +ballvelocity * ((oldPosition.x - playerTopLeft.x) / ballvelocity.x);
      newPosition = oldPosition + cv::Point2f(-1 * ballvelocity.x, ballvelocity.y);// *((newPosition.x - playerTopLeft.x) / ballvelocity.x);
      // the next is necessary cuz of how i wrote my code
      ballDirection = cv::Point2f(-1 * ballDirection.x, 1 * ballDirection.y);
      ballPosition = newPosition;
   }
   if ( (ballBottLeft.x <= PonglordBottRight.x) & ((ballTopLeft.y <= PonglordBottRight.y) & (ballBottLeft.y >= PonglordTopRight.y)) )
   {
      oldPosition = newPosition + -1 * ballvelocity;
      newPosition = oldPosition + cv::Point2f(-1 * ballvelocity.x, ballvelocity.y);
      ballDirection = cv::Point2f(-1 * ballDirection.x, 1 * ballDirection.y);
      ballPosition = newPosition;
   }
   if ((ballTopRight.y <= (float)ScrnTop))
   {
      oldPosition = newPosition + -1 * ballvelocity;// +ballvelocity * ((oldPosition.x - playerTopLeft.x) / ballvelocity.x);
      newPosition = oldPosition + cv::Point2f(ballvelocity.x, -1 * ballvelocity.y);// *((newPosition.x - playerTopLeft.x) / ballvelocity.x);
      // the next is necessary cuz of how i wrote my code
      ballDirection = cv::Point2f(1 * ballDirection.x, -1 * ballDirection.y);
      ballPosition = newPosition;
   }
   if ((ballBottRight.y >= (float)ScrnBott))
   {
      oldPosition = newPosition + -1 * ballvelocity;// +ballvelocity * ((oldPosition.x - playerTopLeft.x) / ballvelocity.x);
      newPosition = oldPosition + cv::Point2f(ballvelocity.x, -1 * ballvelocity.y);// *((newPosition.x - playerTopLeft.x) / ballvelocity.x);
      // the next is necessary cuz of how i wrote my code
      ballDirection = cv::Point2f(1 * ballDirection.x, -1 * ballDirection.y);
      ballPosition = newPosition;
   }
}

float CPong::randomServe() // returns a random number between - 1 and 1
{
   // Seed the random number generator using current time
   auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
   std::mt19937 generator(seed);

   std::uniform_real_distribution<float> distribution(-1.0f, 1.0f); // Generate a random number between -1 and 1

   return distribution(generator);
}

// win condition based on ball centrePoint X-position
void CPong::updateScore(float& ballPositionX, int& Count, bool& reset, int& player, int& Ponglord)
{
   if (ballPositionX < (TopLeft0.x + ballRadius/2))
   { 
      player++;
      Count = 0;
   }
      
   if (ballPositionX > (guiSize.x - ballRadius/2))
   {
      Ponglord++;
      Count = 0;
   }

   if ( (playerScore == winningScore) | (PonglordScore == winningScore) )
   {
      reset = 1;
   }

}