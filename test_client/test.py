#!/usr/bin/python3

import connection
import time

with connection.Connection() as connection:
  connection.calibrate(False, 200000)
  while True:
    connection.absolute_move(10000)
    time.sleep(1.0)
    for i in range(0, 10):
      connection.relative_move(20000)
      time.sleep(0.1)
      connection.relative_move(-10000)
      time.sleep(0.1)
