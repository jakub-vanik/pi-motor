import protocol_pb2
import socket
import struct

class Connection:

  def __init__(self):
    self.connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self.connection.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    self.connection.connect(("127.0.0.1", 1024))

  def __enter__(self):
    return self

  def __exit__(self, exc_type, exc_value, traceback):
    self.connection.close()
    self.connection = None

  def calibrate(self, direction, limit):
    request = protocol_pb2.Request()
    request.calibrate.direction = direction
    request.calibrate.limit = limit
    self.send(request)
    response = protocol_pb2.Response()
    self.receive(response)
    return response.calibrate.success

  def relative_move(self, increment):
    request = protocol_pb2.Request()
    request.relative_move.increment = increment
    self.send(request)
    response = protocol_pb2.Response()
    self.receive(response)

  def absolute_move(self, position):
    request = protocol_pb2.Request()
    request.absolute_move.position = position
    self.send(request)
    response = protocol_pb2.Response()
    self.receive(response)

  def send(self, request):
    size = request.ByteSize()
    data = request.SerializeToString()
    self.connection.sendall(struct.pack("I", size))
    self.connection.sendall(data)

  def receive(self, response):
    size = struct.unpack("I", self.connection.recv(4, socket.MSG_WAITALL))[0]
    data = self.connection.recv(size, socket.MSG_WAITALL)
    response.MergeFromString(data)
