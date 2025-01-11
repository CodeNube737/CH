/**
 * @file CGameObject.h
 * @author Mikhail Rego
 * @date 2024/Feb/29
 * @brief Header file for the CGameObject class.
 */

#pragma once
#include "CControl.h"
#define FALCON_SIZE 30 ///< pixels
#define FALCON_LIVES 10
#define FALCON_THRUST 95 ///< Percent

class CGameObject
{
public:
   CGameObject();
   int get_lives() { return _lives; } //getter
   void set_lives(int lives) { _lives = lives; } //setter
   cv::Point2f get_pos() { return _position; } //getter
   void set_pos(cv::Point2f pos) { _position = pos; } //setter
   void set_radius(int radius) { _radius = radius; } //setter

   void start_up(cv::Mat& im);
   void draw(cv::Mat& im);

protected:
   cv::Point2f _position; ///< The position of any object during any loop.
   cv::Point2f _velocity; ///< The velocity of any object during any loop.
   int _radius; ///< The radius of any object during any loop.
   int _lives; ///< Each type of object has a default amount of lives.

};


// OG code
/*
#include "FPSCounter.h" ///< Include the header for the FPSCounter class
#define WINDOW_NAME "Asteroids"
#define MIN_ROID_SPEED 1.0
#define MAX_ROID_SPEED 30.0
#define MAX_NUMBER_ROIDS 10 ///< Having more asteroids than lives leads to some errors.
#define FALCON_SIZE 30
#define FALCON_LIVES 10
#define FALCON_THRUST 95 ///< Percent
#define PEWPEW_LENGTH 8


class CGameObject
{
protected:
   cv::Point2f _position; ///< The position of any object during any loop.
   cv::Point2f _velocity; ///< The velocity of any object during any loop.
   int _radius; ///< The radius of any object during any loop.
   int _lives; ///< Each type of object has a default amount of lives.
   cv::Size _guiSize; ///< Size of the GUI.

public:
   CGameObject(int radius, int lives, cv::Point2f position, cv::Point2f velocity, cv::Size guiSize) :
      _radius(radius), _lives(lives), _position(position), _velocity(velocity), _guiSize(guiSize)
   {}
   void move();
   cv::Point2f edgeFinder(cv::Point2f direction);
   void collide_wall(cv::Size board);
   void hit();
   int get_lives() { return _lives; } //getter
   void set_lives(int lives) { _lives = lives; } //setter
   int get_pos() { return _position; } //getter
   void set_pos(cv::Point2f pos) { _position = pos; } //setter
   void set_radius(int radius) { _radius = radius; } //setter
   void draw(cv::Mat& im);
};
*/