/**
 * @file CSketch.h
 * @brief This file contains all definitions and functions required for running Etch-a-Sketch.
 */

#pragma once

#include "CBase4618.h"

#define INCREMENT 8 ///< The speed of the sketch movement.

/**
 * @class CSketch
 * @brief The class containing all definitions and functions required for running Etch-a-Sketch.
 * @details This class inherits from the CBase4618 class.
 */
class CSketch : public CBase4618 {
private:
   int flagA = 1; ///< Flag for resetting logic.
   int flagB = 1; ///< Flag for quit logic.
   int reset = 0; ///< Flag for resetting logic.
   int quit = 0; ///< Flag for quit logic.
   int count = 0; ///< Count variable.

   int pt1x = 375; ///< Start point for line (x).
   int pt1y = 375; ///< Start point for line (y).
   int pt2x = 375; ///< End point for line (x).
   int pt2y = 375; ///< End point for line (y).

public:
   /**
    * @brief Constructor that passes the elastic GUI size.
    * @param size The size of the GUI.
    */
   CSketch(int size);

   /**
    * @brief Draws the new line every CPU cycle in the while loop, as well as resets.
    * @note This is a pure virtual function.
    */
   virtual void draw();

   /**
    * @brief Grabs the position of the joystick as well as updates the digital firmware.
    * @note This is a pure virtual function.
    */
   virtual void update();

   virtual void run();
};
