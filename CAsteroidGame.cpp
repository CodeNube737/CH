//CAsteroidGame.cpp

#include "stdafx.h" // must be added above all code in external .ccp files when using the template
#include "cvui.h" // for lab4 onward. To be included in .cpp file, not .h file
#include "CAsteroidGame.h" 

CAsteroidGame::CAsteroidGame(int scaleFactor = 100) :
   reset(false), quit(false), fps(0), //global
   guiSize(10 * scaleFactor, 8 * scaleFactor) //gui
{
   _canvas = cv::Mat(guiSize, CV_8UC3); // main window... shrink to be as small as possible
   control.init_com(COMPORT);

   // *** u need to totally restructure the way you setup you GameObject vs AsteroidGame, using the principles of OOP.
   //MilleniumFalcon._lives = FALCON_LIVES;
   //MilleniumFalcon._radius = FALCON_SIZE / 2;
   //MilleniumFalcon._position = cv::Point2f(guiSize.width / 2, guiSize.height / 2);
}

void CAsteroidGame::run()
{
   quit = false;

   std::thread drawThread(&CAsteroidGame::draw, this);
   std::thread updateThread(&CAsteroidGame::update, this);

   drawThread.join();
   updateThread.join();

   while (!quit)
      quit = cv::waitKey(1);
   
   exit(0);
}

void CAsteroidGame::update()
{
   while (!quit)
   {
      // should read inputs from controller, then apply them to the objects directly

      std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_THREAD));
   }
}

void CAsteroidGame::draw()
{
   cvui::init(WINDOW_NAME, 20); // init main window

   while (!quit)
   {
      auto startTime = std::chrono::high_resolution_clock::now(); // start FPS timer

      // setup the menu
      _canvas = cv::Scalar(49, 52, 49); // clear the _canvas
      if (cv::waitKey(1) && (cvui::button(_canvas, 10, 10, "&Quit ")))
         quit = true;
      if (cv::waitKey(1) && (cvui::button(_canvas, 10, 40, "&Reset")))
         reset = true;
      cvui::printf(_canvas, 10, 70, "F.P.S. = %f", fps);

      // *** u need to totally restructure the way you setup you GameObject vs AsteroidGame, using the principles of OOP.
      // draw elements
      //cv::circle(_canvas, MilleniumFalcon._position.x, )



      cvui::update(); // ui update
      cv::imshow(WINDOW_NAME, _canvas); // Update the window where the trackbars are

      // setup fps and delay
      std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_THREAD));
      auto endTime = std::chrono::high_resolution_clock::now();
      u_int duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
      fps = 1000.0 / duration; // convert duration to seconds before calculating fps
   }
}



//OG Code
/*

CAsteroidGame::CAsteroidGame(int scaleFactor = 100) :
   //global
   reset(false), quit(false), 
   //gui
   guiSize(10 * scaleFactor, 8 * scaleFactor),
   MilleniumFalcon(FALCON_SIZE, FALCON_LIVES, cv::Point2f(guiSize.width / 2, guiSize.height / 2), cv::Point2f(0, 0), guiSize)
{
   control.init_com(COMPORT);
   CMissile laser(PEWPEW_LENGTH, 1, cv::Point(0, 0), cv::Point(0, 30), guiSize);
   // Add asteroids to the vector
   for (int i = 0; i < MAX_NUMBER_ROIDS; i++) { // change the number of asteroids as needed
      // Create a new asteroid with random properties
      CAsteroid asteroid(rand() % 50 + FALCON_SIZE + 1, 1, cv::Point2f(rand() % (int)guiSize.width, 0), cv::Point2f((rand() % 10) - 5, (rand() % 10) - 5), guiSize);
      itsaTrap.push_back(asteroid); // Add the asteroid to the vector
   }
}

void CAsteroidGame::run()
{
   using namespace cv;
   quit = false;

   //bool check = true;
  
   std::thread drawThread(&CAsteroidGame::draw, this);
   std::thread updateThread(&CAsteroidGame::update, this);
   //std::thread tunesThread(&CAsteroidGame::tunes, this);

   // Ensure the drawThread and updateThread have completed their execution before the next iteration
   drawThread.join();
   updateThread.join();
   //tunesThread.join();

   while (!quit)
   {
      quit = waitKey(1);
   }
   exit(0);
}

void CAsteroidGame::update() {
   while (!quit)
   {
      //fpsCounter.update(); // Update FPS counter
      int shipLife = MilleniumFalcon.get_lives();

      // update the position of all objects
      for (auto& asteroid : itsaTrap) {
         cv::Point2f roidPos;
         roidPos = asteroid.get_pos();
         asteroid.set_pos(roidPos);

         int get;// just cuz i made an akward get setup
         cv::Point2f shipPos = MilleniumFalcon.get_pos() + cv::Point2f( (control.get_analog(ANALOG, JOY_X, get)-50)*1, (control.get_analog(ANALOG, JOY_Y, get)-50)*-1 ) * FALCON_THRUST / 500;
         MilleniumFalcon.set_pos(shipPos); // this is the proper way to change a private or protected variable in a class

         cv::Point2f pewPos;
         if (control.get_button(DIGITAL, SW2, get)) { // pewPew 
            CMissile laser(PEWPEW_LENGTH, 1, MilleniumFalcon.get_pos(), cv::Point(0, -30), guiSize);
            pewPew.push_back(laser);
            pewPos = laser.get_pos();
         }

         // now u can blow things up
         cv::Point2f deflectionAngle = shipPos - roidPos; // note: direction goes from asteroid to ship!
         float deflectionABS = sqrt(deflectionAngle.x * deflectionAngle.x + deflectionAngle.y * deflectionAngle.y);
         cv::Point2f direction = deflectionAngle / deflectionABS;
         cv::Point2f shipEdge = MilleniumFalcon.edgeFinder(direction);
         cv::Point2f roidEdge = asteroid.edgeFinder(direction);
         cv::Point2f overlap = shipEdge - roidEdge;
         if ((overlap.x < 20) & (overlap.x > -20) & (overlap.y < 20) & (overlap.y > -20))
         {
            shipLife = MilleniumFalcon.get_lives();
            MilleniumFalcon.set_lives((--shipLife));
             if (!shipLife)
                MilleniumFalcon.hit();

            int roidLife = asteroid.get_lives();
            asteroid.set_lives((--roidLife));
             if (!roidLife)
                asteroid.hit();
         }
      }

      // Remove all asteroids with zero life
      for (int i = itsaTrap.size() - 1; i >= 0; --i)
      {
         if (itsaTrap[i].get_lives() == 0)
            itsaTrap.erase(itsaTrap.begin() + i);
      }
   }
}

void CAsteroidGame::draw() {
   cvui::init(WINDOW_NAME, 20);
   cv::Mat _canvas = cv::Mat(cv::Size(guiSize.width, guiSize.height), CV_8UC3); // instantiate mtrx (size(width, height), type)

   while (!quit)
   {
      fpsCounter.update(); // Update FPS counter

      _canvas = cv::Scalar(49, 52, 49); // clear the _canvas
      if (cv::waitKey(1) && (cvui::button(_canvas, 10, 10, "&Quit"))) // quit button
         quit = true;

      if ((cvui::button(_canvas, 10, 80, "Reset")))
      {
         // Reset the game state
         reset = 0;

         // Reset the ship
         MilleniumFalcon.set_lives(FALCON_LIVES);
         MilleniumFalcon.set_pos(cv::Point2f(guiSize.width / 2, guiSize.height / 2));
         MilleniumFalcon.set_radius(FALCON_SIZE);

         // Clear the asteroids vector
         itsaTrap.clear();

         // Clear the missile vector
         pewPew.clear();

         // Add new asteroids to the vector
         for (int i = 0; i < MAX_NUMBER_ROIDS; i++) {
            // Create a new asteroid with random properties
            CAsteroid asteroid(rand() % 50 + FALCON_SIZE + 1, 1, cv::Point2f(rand() % (int)guiSize.width, 0), cv::Point2f((rand() % 10) - 5, (rand() % 10) - 5), guiSize);
            itsaTrap.push_back(asteroid); // Add the asteroid to the vector
         }
      }

      // draw the asteroids
      for (auto& asteroid : itsaTrap) {
         asteroid.draw(_canvas);
         asteroid.move();
         asteroid.collide_wall(guiSize);
      }
      // draw the ship
      MilleniumFalcon.draw(_canvas);
      MilleniumFalcon.collide_wall(guiSize);

      // draw the lasers
      for (auto& laser : pewPew) {
         laser.draw(_canvas);
         laser.move();
         laser.collide_wall(guiSize);
      }

      int shipLives = MilleniumFalcon.get_lives();
      cvui::printf(_canvas, 90, 15, "Frames/sec: %f", fpsCounter.FPS);
      cvui::printf(_canvas, 90, 35, "Lives: %d", shipLives);
      cvui::update();
      cv::imshow(WINDOW_NAME, _canvas);

   }
}

void CAsteroidGame::tunes()
{
   //if (!music.openFromFile("C:\\Users\\Owner\\Downloads\\ELEX4618-Template\\Fail Recorder_ Mission Impossible Themesong.ogg"))
      std::cout << "music file not found...\n";
   
   //music.play();

   //music.stop(); // actually not needed...
}
*/