/**
 * @file CShip.h
 * @author Mikhail Rego
 * @brief Header file for the CMissile class.
 */
#pragma once
#include "CGameObject.h"


class CShip : public CGameObject
{
public:
   CShip() {}
};


// OG code
/*

class CShip : public CGameObject
{
public:
   CShip(int, int, cv::Point2f, cv::Point2f, cv::Size);
private:
};
*/