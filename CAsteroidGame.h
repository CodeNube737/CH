/**
 * @file CAsteroidGame.h
 * @author Mikhail Rego
 * oDate 2024/Feb/29 (original)
 * @date 2024/Apr/16
 * @brief Header file for the CAsteroidGame class.
 */
#pragma once
#include "CBase4618.h"
#include "CShip.h"
//#include "CMissile.h"
//#include "CAsteroid.h"
#define DELAY_THREAD 20 // miliseconds
#define WINDOW_NAME "Asteroids"

class CAsteroidGame : public CBase4618
{
public:
   CAsteroidGame(int scaleFactor);
   virtual void run();
   virtual void draw();
   virtual void update();
   //void fpsUpdate();

protected:
   bool reset; ///< Reset flag to restart the game.
   bool quit; ///< Quit flag to quit the game.
   float fps;
   cv::Size guiSize; ///< Size of the GUI, proportional to scaleFactor
   CShip MilleniumFalcon; ///< Only one ship, so vector is not needed
};



// OG code
/*
//#include <SFML/Audio.hpp>


class CAsteroidGame : public CBase4618
{
protected:
   bool reset; ///< Reset flag to restart the game.
   bool quit; ///< Quit flag to quit the game.
   FPSCounter fpsCounter; ///< Instance of the FPSCounter class.
   cv::Size guiSize; ///< Size of the GUI, proportional to scaleFactor.
   std::vector<CAsteroid> itsaTrap; ///< Vector to hold all the asteroids.
   std::vector<CMissile> pewPew; ///< Vector to hold all the missiles/lasers.
   CShip MilleniumFalcon; ///< Only one ship, so vector is not needed.

public:
   CAsteroidGame(int scaleFactor);
   //sf::Music music; ///< object for playing music through SFML
   //std::mutex mtx;  ///< Mutex for serial port access

   virtual void draw();
   virtual void update();
   virtual void run();
   void tunes();
};
*/