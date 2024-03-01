#include "application.h"

#include <chrono>
#include <iostream>
#include <thread>

Application::Application() : motor_shield(100000), server(motor_shield) {}

void Application::run()
{
  std::unique_lock<std::mutex> lock(mutex);
  std::cout << "starting" << std::endl;
  server.start();
  std::cout << "started" << std::endl;
  condition_variable.wait(lock);
  server.stop();
  std::cout << "terminated" << std::endl;
}

void Application::terminate()
{
  std::cout << "terminating" << std::endl;
  condition_variable.notify_one();
}
