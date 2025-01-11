
#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "cvui.h" // for lab4 onward. To be included in .cpp file, not .h file
#include "CSketch.h" // contains no definitions, not even object initializations. _canvas is intialized here

CSketch::CSketch(int size)
{
	cvui::init("Test");
	_canvas = cv::Mat::zeros(size, size, CV_8UC3);
}

void CSketch::run()
{
	using namespace cv;
	int eXit;

	do
	{
		draw();
		update();
		eXit = waitKey(1);
	} while (eXit != 'q'); // TaeWoo and Greg advise NOT to worry about why this is screwing-up your GUI exit button

	exit(0);
}

void CSketch::update()
{
	int get;// just cuz i made an akward get setup
	if ((int)control.get_analog(ANALOG,JOY_X,get) > 60)
	{
		pt1x += INCREMENT;
	}

	else if ((int)control.get_analog(ANALOG, JOY_X, get) < 40)
	{
		pt1x -= INCREMENT;
	}

	else
	{
		pt1x = pt1x;
	}

	if ((int)control.get_analog(ANALOG, JOY_Y, get) > 60)
	{
		pt1y -= INCREMENT;
	}

	else if ((int)control.get_analog(ANALOG, JOY_Y, get) < 40 )
	{
		pt1y += INCREMENT;
	}

	else
	{
		pt1y = pt1y;
	}

	if (pt1x >= 750)
	{
		pt1x = 750;
	}

	if (pt1x <= 0)
	{
		pt1x = 0;
	}

	if (pt1y >= 750)
	{
		pt1y = 750;
	}

	if (pt1y <= 0)
	{
		pt1y = 0;
	}

	int Butt1Press;

	control.get_data(DIGITAL, SW2, Butt1Press);

	if (Butt1Press == 0 && flagA == 1)
	{
		count++;
		flagA = 0;

		if (count > 2)
		{
			count = 0;
		}
	}

	if (Butt1Press == 1 && flagA == 0)
	{
		flagA = 1;
	}

	int Butt2Press;

	control.get_data(DIGITAL, SW1, Butt2Press);

	if (Butt2Press == 0 && flagB == 1)
	{
		reset = 1;
	}

	if (Butt2Press == 1 && flagB == 0)
	{
		flagA = 1;
	}

	//int Zin;
	//int Xin;
	//control.get_data(1, 5, Zin);
	//control.get_data(1, 7, Xin);

	//if (Zin > 3800 || Xin > 3800)
	//{
	//	_reset = 1;
	//}

	if (cvui::button(_canvas, 30, 100, "RESET"))
	{
		reset = 1;
	}

	if (cvui::button(_canvas, 100, 100, "QUIT"))
	{
		quit = 1;
	}

	std::cout << (cvui::button(_canvas, 30, 100, "RESET")) << "\n"; // don't see the point of this in theory...
}

void CSketch::draw()
{
	cvui::imshow("Test", _canvas);
	cvui::window(_canvas, 3, 3, 160, 135, "Etch_a_Sketch");

	if (count == 0)
	{
		control.set_data(DIGITAL, LED_R, 0);
		control.set_data(DIGITAL, LED_G, 0);
		control.set_data(DIGITAL, LED_B, 1);
		cvui::text(_canvas, 10, 50, "Color: BLUE");
		Sleep(5);
		cv::line(_canvas, cv::Point(pt1x, pt1y), cv::Point(pt2x, pt2y), cv::Scalar(255, 0, 0), 3);
	}

	else if (count == 1)
	{
		control.set_data(DIGITAL, LED_R, 0);
		control.set_data(DIGITAL, LED_G, 1);
		control.set_data(DIGITAL, LED_B, 0);
		cvui::text(_canvas, 10, 50, "Color: GREEN");
		Sleep(5);
		cv::line(_canvas, cv::Point(pt1x, pt1y), cv::Point(pt2x, pt2y), cv::Scalar(0, 255, 0), 3);
	}

	else
	{
		control.set_data(DIGITAL, LED_R, 1);
		control.set_data(DIGITAL, LED_G, 0);
		control.set_data(DIGITAL, LED_B, 0);
		cvui::text(_canvas, 10, 50, "Color: RED");
		Sleep(5);
		cv::line(_canvas, cv::Point(pt1x, pt1y), cv::Point(pt2x, pt2y), cv::Scalar(0, 0, 255), 3);
	}

	if (reset == 1)
	{
		_canvas = cv::Mat::zeros(750, 750, CV_8UC3);
		reset = 0;
	}

	if (quit == 1)
	{
		exit(0);
	}

	pt2x = pt1x;
	pt2y = pt1y;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
//constructor
CSketch::CSketch()
{
   cvui::init("myController");

   _canvas = cv::Mat::zeros(cv::Size(800, 800), CV_8UC3);

   myController.init_com(3);

   cv::Point prevEndPoint((myController.get_analog(ANALOG,11,x_old) * 8), (800 - (myController.get_analog(ANALOG,4,y_old) * 8))); // i'm putting-in a lot of dummy variables, cuz i'm running out of time.
   cv::Point newEndPoint = prevEndPoint;
   int& tempx = x_old;
   int& tempy = y_old;
   cv::Point temp(tempx, tempy);
   cv::Scalar lineColor(255, 0, 0);
   
   while (true)
   {
      x_new = myController.get_analog(ANALOG, myController.channelx, x_old);
      y_new = myController.get_analog(ANALOG, myController.channely, y_old);
      //temp = update(lineColor);
      //temp.tempx = 

      if (x_new < 238 && newEndPoint.x >= 5)
         newEndPoint.x = prevEndPoint.x - 10;
      if (x_new < 490 && newEndPoint.x >= 790)
         newEndPoint.x = prevEndPoint.x + 10;
      if (y_new < 238 && newEndPoint.y >= 10)
         newEndPoint.y = prevEndPoint.y - 10;
      if (y_new < 490 && newEndPoint.y >= 790)
         newEndPoint.y = prevEndPoint.y + 10;

      cv::Point startPoint = prevEndPoint;
      cv::Point endPoint = newEndPoint;

      draw(_canvas, startPoint, endPoint, lineColor);

      cvui::window(_canvas, 30, 10, 150, 150, "Setting"); 

      cvui::text(_canvas, 40, 130, "Colour", 0.4, starting_colour);

      if (lineColor == cv::Scalar(0, 255, 0))
      {
         starting_colour = 0x00ff00;
         myController.set_data(DIGITAL, 39, 0);
         myController.set_data(DIGITAL, 38, 1);
         myController.set_data(DIGITAL, 37, 0);
      }

      if (lineColor == cv::Scalar(255, 0, 0))
      {
         starting_colour = 0x0000ff;
         myController.set_data(DIGITAL, 39, 0);
         myController.set_data(DIGITAL, 38, 0);
         myController.set_data(DIGITAL, 37, 1);
      }

      if (lineColor == cv::Scalar(0, 0, 255))
      {
         starting_colour = 0xff0000;
         myController.set_data(DIGITAL, 39, 1);
         myController.set_data(DIGITAL, 38, 0);
         myController.set_data(DIGITAL, 37, 0);
      }

      if (cvui::button(_canvas, 40, 40, 130, 130, "Quit This Program"))
      {
         exit(0);
      }
      if (cvui::button(_canvas,40,80,130,130,"Reset"))
      {
         _canvas = cv::Mat::zeros(cv::Size(800, 800), CV_8UC3);
      }

      cvui::update();

      cv::imshow("myController", _canvas);

      if (cv::waitKey(20) == 113)
      {
         exit(0);
      }

      prevEndPoint = newEndPoint;
   }
}

//destructor
CSketch::~CSketch() {}

// draw
void CSketch::draw(cv::Mat& canvas, const cv::Point& startPoint, const cv::Point& endPoint, const cv::Scalar& colour)
{
   cv::line(canvas, startPoint, endPoint, colour, 10);
}

// update
int CSketch::update(cv::Scalar& lineColour)
{
   int x_new = (myController.get_analog(ANALOG, myController.channelx, x_old));
   int y_new = (myController.get_analog(ANALOG, myController.channely, y_old));

   myController.get_data(DIGITAL, myController.channel_debounce, reset_PB);
   myController.get_data(DIGITAL, myController.channel_digital, colour_PB);
   //myController.get_data(ANALOG, channel_accelerometer, reset_accelerometer);

   if (reset_PB == 0)
   {
      _canvas = cv::Mat::zeros(cv::Size(800, 800), CV_8UC3);
      myController.get_button(DIGITAL, myController.channel_debounce, reset_PB);
   }

   if (colour_PB == 0)
   {
      if (lineColour == cv::Scalar(255, 0, 0)) {
         lineColour == cv::Scalar(0, 255, 0);
      }

      else if (lineColour == cv::Scalar(0, 255, 0)) {
         lineColour == cv::Scalar(0, 0, 255);
      }//

      else lineColour == cv::Scalar(255, 0, 0);
   }

   //std::cout << reset_accelerometer << std::endl;
   return 0;
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// run... may just need to cave-in and do it that way
//void run() //{}
/*
{
   while (true)
   {
      draw(cv::Mat& _Canvas = 0, const cv::Point& _startPoint, const cv::Point& _endPoint, const cv::Scalar& _colour);
      update();

      if (_kbhit())
      {
         ExitKey = getch();
         if (ExitKey == 'q') { exit(0); }
      }
   }
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
// constructor
//CSketch::CSketch() { }
CSketch::CSketch(cv::Size canvasSize, int comPort) : CBase4618(comPort), _canvasSize(canvasSize) {
   _canvas = cv::Mat::zeros(_canvasSize, CV_8UC3);
}

// run
void CSketch::run() 
{
   // Implementation...
}

// update
void CSketch::update() 
{
   // Implementation...
}

// draw

void CSketch::draw() 
{
   // Implementation...
}

*/