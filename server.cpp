#include "server.h"

#include "connection.h"

Server::Server(MotorShield &motor_shield) :
  Poco::Net::TCPServer(new ConnectionFactory(motor_shield), 1024)
{
}

Server::ConnectionFactory::ConnectionFactory(MotorShield &motor_shield) :
  motor_shield(motor_shield)
{
}

Poco::Net::TCPServerConnection *Server::ConnectionFactory::createConnection(
    const Poco::Net::StreamSocket &socket)
{
  return new Connection(socket, motor_shield);
}
