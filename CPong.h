/**
 * @file CPong.h
 * @author Mikhail Rego
 * @date 2024/Feb/29
 */

#pragma once
#include "CBase4618.h"
#include "FPSCounter.h" ///< Include the header for the FPSCounter class
#define WINDOW_NAME "CVUI Test"

 /**
  * @class CPong
  * @brief A class that inherits from CBase4618
  */
class CPong : public CBase4618
{
private:
   bool reset; ///< reset flag
   bool quit; ///< quit flag
   int count; ///< count integer
   float rand1; ///< 1st float for generating random velocity
   float rand2; ///< 2nd float for generating random velocity
   FPSCounter fpsCounter; ///< Create an instance of the FPSCounter class

public:
   /**
    * @brief Constructor for CPong class
    * @param scaleFactor scaleFactor to be used in constructor
    */
   CPong(int scaleFactor);
   cv::Point2f guiSize; ///< proportional to scalefactor
   cv::Point2f TopLeft0; ///< the origin of cvui windows
   double ballRadius; ///< variable with slider
   cv::Point2f ballCentre; ///< variable with velocity
   cv::Point2f ballTopLeft; ///< = ballCentre + cv::Point(-1*ballRadius, -1*ballRadius)
   cv::Point2f ballBottLeft; ///< = ballCentre + cv::Point(-1*ballRadius, 1*ballRadius)
   cv::Point2f ballTopRight; ///< = ballCentre + cv::Point(1*ballRadius, -1*ballRadius)
   cv::Point2f ballBottRight; ///< = ballCentre + cv::Point(1*ballRadius, 1*ballRadius)
   int ballSpeed;
   cv::Point2f paddleSize; ///< = cv::Point(scaleFactor / 5, scaleFactor * 1.5)
   cv::Point2f playerCenter; ///< x is constant; y is variable with joystick_y (update..??)
   cv::Point2f playerTopLeft; ///< = playerCentre + cv::Point(paddleSize.x / -2, paddleSize.y / -2)
   cv::Point2f playerBottLeft; ///< = playerCentre + cv::Point(paddleSize.x / -2, paddleSize.y / 2)
   cv::Point2f PonglordCenter; ///< x is constant. y is either = ballCentre.y; OR is a function of joystick sensitivity and ballCentre.y
   cv::Point2f PonglordTopRight; ///< = pongLordCentre + cv::Point(paddleSize.x / 2, paddleSize.y / -2)
   cv::Point2f PonglordBottRight; ///< = pongLordCentre + cv::Point(paddleSize.x / 2, paddleSize.y / 2)
   int playerScore; ///< keeps track of player's score
   int PonglordScore; ///< keeps track of CPU's score
   int winningScore; ///< score that wins the game score
   float Ponglordskill; ///< adjustable CPU difficulty (based on speed)
   cv::Point2f DefaultBall; ///< = cv::Point(guiSize.x / 2, guiSize.y / 2)
   cv::Point2f DefaultPlayer; ///< = cv::Point(guiSize.x - scaleFactor / 10, guiSize.y / 2)
   cv::Point2f DefaultPonglord; ///< = cv::Point(TopLeft.x + scaleFactor / 10, guiSize.y / 2)
   int ScrnTop; ///< = TopLeft0.y
   int ScrnRight; ///< = guiSize.x
   int ScrnBott; ///< = guiSize.y
   int ScrnLeft; ///< = TopLeft0.x
   cv::Point2f ballDirection; ///< direction of the ball based on its velovity
   cv::Point2f ballvelocity; ///< velocity of the ball based on previous position
   float t; ///< a float for delta-t multiplier
   cv::Point2f newPosition; ///< duh
   cv::Point2f oldPosition; ///< duh

   /**
    * @brief Updates the positions of the paddles
    * @param float&
    * @param float&
    */
   void updatePaddles(float&, float&);

   /**
    * @brief Updates the position of the ball
    * @param cv::Point2f&
    */
   void updateBall(cv::Point2f&);

   /**
    * @brief Serves the ball at the start of a game in a random direction
    * @return float
    */
   float randomServe();

   /**
    * @brief Updates the scores
    * @param float&
    * @param int&
    * @param bool&
    * @param int&
    * @param int&
    */
   void updateScore(float&, int&, bool&, int&, int&);

   /**
    * @brief Draws the game
    */
   virtual void draw();

   /**
    * @brief Updates the game
    */
   virtual void update();

   /**
    * @brief Runs the game
    */
   virtual void run();
};
