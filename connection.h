#pragma once

#include "motor_shield.h"
#include "protocol.pb.h"

#include <Poco/Net/TCPServerConnection.h>

class Connection : public Poco::Net::TCPServerConnection
{
public:
  Connection(const Poco::Net::StreamSocket &socket, MotorShield &motor_shield);
  void run();

private:
  MotorShield &motor_shield;
  bool receiveRequest(protocol::Request &request);
  bool transmitResponse(protocol::Response &response);
  void processCalibrate(protocol::CalibrateReq *request,
                        protocol::CalibrateResp *response);
  void processRelativeMove(protocol::RelativeMoveReq *request,
                           protocol::RelativeMoveResp *response);
  void processAbsoluteMove(protocol::AbsoluteMoveReq *request,
                           protocol::AbsoluteMoveResp *response);
};
