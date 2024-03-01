#include "connection.h"

Connection::Connection(const Poco::Net::StreamSocket &socket,
                       MotorShield &motor_shield) :
  Poco::Net::TCPServerConnection(socket), motor_shield(motor_shield)
{
}

void Connection::run()
{
  while (true) {
    protocol::Request request;
    protocol::Response response;
    if (!receiveRequest(request)) {
      break;
    }
    switch (request.request_case()) {
      case protocol::Request::RequestCase::kCalibrate:
        processCalibrate(request.mutable_calibrate(),
                         response.mutable_calibrate());
        break;
      case protocol::Request::RequestCase::kRelativeMove:
        processRelativeMove(request.mutable_relative_move(),
                            response.mutable_relative_move());
        break;
      case protocol::Request::RequestCase::kAbsoluteMove:
        processAbsoluteMove(request.mutable_absolute_move(),
                            response.mutable_absolute_move());
        break;
    }
    if (!transmitResponse(response)) {
      break;
    }
  }
}

bool Connection::receiveRequest(protocol::Request &request)
{
  try {
    uint32_t size;
    if (socket().receiveBytes(&size, sizeof(size), MSG_WAITALL) !=
        sizeof(size)) {
      return false;
    }
    char data[size];
    if (socket().receiveBytes(data, size, MSG_WAITALL) != size) {
      return false;
    }
    if (!request.ParseFromArray(data, size)) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  }
}

bool Connection::transmitResponse(protocol::Response &response)
{
  try {
    uint32_t size = response.ByteSizeLong();
    if (socket().sendBytes(&size, sizeof(size), MSG_WAITALL) != sizeof(size)) {
      return false;
    }
    char data[size];
    if (!response.SerializePartialToArray(data, size)) {
      return false;
    }
    if (socket().sendBytes(data, size, MSG_WAITALL) != size) {
      return false;
    }
    return true;
  } catch (...) {
    return false;
  }
}

void Connection::processCalibrate(protocol::CalibrateReq *request,
                                  protocol::CalibrateResp *response)
{
  response->set_success(
      motor_shield.calibrate(request->direction(), request->limit()));
}

void Connection::processRelativeMove(protocol::RelativeMoveReq *request,
                                     protocol::RelativeMoveResp *response)
{
  response->set_position(motor_shield.relativeMove(request->increment()));
}

void Connection::processAbsoluteMove(protocol::AbsoluteMoveReq *request,
                                     protocol::AbsoluteMoveResp *response)
{
  motor_shield.absoluteMove(request->position());
}
