#pragma once

#include "motor_shield.h"

#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnectionFactory.h>

class Server : public Poco::Net::TCPServer
{
public:
  Server(MotorShield &motor_shield);

private:
  class ConnectionFactory : public Poco::Net::TCPServerConnectionFactory
  {
  public:
    ConnectionFactory(MotorShield &motor_shield);
    Poco::Net::TCPServerConnection *
    createConnection(const Poco::Net::StreamSocket &socket);

  private:
    MotorShield &motor_shield;
  };
};
