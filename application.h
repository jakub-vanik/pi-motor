#pragma once

#include "motor_shield.h"
#include "server.h"

#include <condition_variable>
#include <mutex>

class Application
{
public:
  Application();
  void run();
  void terminate();

private:
  std::mutex mutex;
  std::condition_variable condition_variable;
  MotorShield motor_shield;
  Server server;
};
