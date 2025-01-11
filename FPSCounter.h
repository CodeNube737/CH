// FPSCounter.h
#pragma once
#include "CBase4618.h"

class FPSCounter {
private:
   std::chrono::high_resolution_clock::time_point lastFrameTime;
   int frames = 0;
   double fps = 0.0;
   double targetFPS = 30.0; // Set your target FPS here

public:
   FPSCounter() {
      lastFrameTime = std::chrono::high_resolution_clock::now();
   }

   double delay = 1000 / targetFPS; // Initial delay is based on target FPS
   double FPS = 0;
   u_int duration;

   void update() {
      frames++;
      auto currentTime = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime).count();
      if (duration >= delay) {
         fps = static_cast<double>(frames) / (duration / 1000.0);
         FPS = fps;
         frames = 0;
         lastFrameTime = currentTime;

         // Adjust delay to try to maintain target FPS
         if (fps < targetFPS) {
            delay -= 1;
         }
         else if (fps > targetFPS) {
            delay += 1;
         }
      }
      // Sleep for the remaining time to maintain the target FPS
      std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay)));
   }
};
