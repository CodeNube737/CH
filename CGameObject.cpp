//CGameObject.cpp
#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "cvui.h" // for lab4 onward. To be included in .cpp file, not .h file
#include "CGameObject.h"


CGameObject::CGameObject() {}// goes in .ccp

void CGameObject::draw(cv::Mat& im)
{
   if (_radius == FALCON_SIZE)
      cv::circle(im, _position, _radius, cv::Scalar(0, 165, 255), CVUI_FILLED); // ship
}


//OG Code
/*

void CGameObject::draw(cv::Mat& im)
{
   if (_radius == FALCON_SIZE)
      cv::circle(im, _position, _radius, cv::Scalar(0, 165, 255), CVUI_FILLED); // ship

   else if (_radius == PEWPEW_LENGTH)
      cv::line(im, _position, cv::Point(_position.x, _position.y - _radius), cv::Scalar(0x0, 0x00, 0xff), 1); // laser/missile

   else 
      cv::circle(im, _position, _radius, cv::Scalar(0x88, 0x88, 0x88)); // asteroid
   
}

void CGameObject::move()
{
   if (_radius == FALCON_SIZE)
   {
      //_position.x++;
   }

   //else if (_radius == ??) {}

   else
      _position += _velocity;// i don't see the need for delta-t...
}
 
void CGameObject::collide_wall(cv::Size board)
{
   if (_position.x > board.width)
      _position.x = 0;
   if (_position.x < 0)
      _position.x = board.width;
   if (_position.y > board.height)
      _position.y = 0;
   if (_position.y < 0)
      _position.y = board.height;
}

cv::Point2f CGameObject::edgeFinder(cv::Point2f direction)
{
   cv::Point2f edge;// = obj._position;
   if (_radius == FALCON_SIZE)
      edge = _position - direction*_radius; //may be plus
   
   else
      edge = _position + direction * _radius; //may be minus

   return edge;
}

void CGameObject::hit()
{
   _radius = 0;
}

*/