#pragma once

#include "clk.h"
#include "dma.h"
#include "gpio.h"
#include "mailbox.h"
#include "memory.h"
#include "pwm.h"
#include "trinamic.h"

#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <mutex>

class MotorShield
{
public:
  MotorShield(uint32_t freq);
  virtual ~MotorShield();
  bool calibrate(bool direction, uint32_t limit);
  uint32_t relativeMove(int32_t increment);
  void absoluteMove(uint32_t position);

private:
  static const size_t dma_words = 128;
  static const uint8_t en_pin = 5;
  static const uint8_t dir_pin = 17;
  static const uint8_t step_pin = 12;
  std::mutex mutex;
  Memory memory;
  Mailbox mailbox;
  Gpio gpio;
  Clk clk;
  Pwm pwm;
  Dma dma;
  Trinamic trinamic;
  bool reverse;
  uint32_t state;
  void rotate(bool direction, uint32_t count);
};
