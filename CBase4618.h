/**
 * @file CBase4618.h
 * @brief This file is a base class to be inherited for future projects that use GUI.
 */

#pragma once

#include "CControl.h"
#include "string.h"

/**
 * @class CBase4618
 * @brief Base class to be inherited for future projects that use GUI.
 */
class CBase4618 {
protected:
   CControl control; ///< Instantiate microcontroller.
   cv::Mat _canvas; ///< Make a GUI object called _canvas.

public:
   /**
    * @brief Take controller input.
    * @note This is a pure virtual function.
    */
   virtual void update() = 0;

   /**
    * @brief Update GUI based on update.
    * @note This is a pure virtual function.
    */
   virtual void draw() = 0;

   /**
    * @brief Runs update and draw in an infinite loop.
    */
   virtual void run() = 0;
};
