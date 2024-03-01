#include "motor_shield.h"

#include "dma_buffer.h"

#include <chrono>
#include <cmath>
#include <thread>
#include <tuple>
#include <vector>

MotorShield::MotorShield(uint32_t freq) :
  gpio(memory),
  clk(memory),
  pwm(memory),
  dma(memory, mailbox, dma_words, 5),
  trinamic("/dev/spidev0.0")
{
  reverse = false;
  state = 0;
  pwm.stop();
  clk.stop();
  clk.start(freq);
  pwm.startSerial();
  gpio.setLevel(en_pin, true);
  gpio.setOutput(en_pin, true);
  gpio.setLevel(dir_pin, false);
  gpio.setOutput(dir_pin, true);
  gpio.setFunction(step_pin, 0);
  trinamic.setup();
  trinamic.setDoubleEdge(true);
  gpio.setLevel(en_pin, false);
}

MotorShield::~MotorShield()
{
  trinamic.disable();
  gpio.setOutput(en_pin, false);
  gpio.setOutput(dir_pin, false);
  gpio.setOutput(step_pin, false);
  pwm.stop();
  clk.stop();
}

bool MotorShield::calibrate(bool direction, uint32_t limit)
{
  std::lock_guard<std::mutex> guard(mutex);
  bool success = false;
  trinamic.setStealthChop(false);
  gpio.setLevel(dir_pin, direction);
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  dma.setup();
  static const uint32_t step = 64;
  for (uint32_t count = 0; count < limit; count += step) {
    uint32_t *data = dma.getData();
    data[0] = 0x55555555;
    data[1] = 0x00000000;
    dma.transfer(0, 2, step);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    uint32_t load = trinamic.getLoad();
    dma.waitReady();
    if (count > 0 && load == 0) {
      success = true;
      break;
    }
  }
  pwm.waitFifoEmpty();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  gpio.setLevel(dir_pin, !direction);
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  uint32_t *data = dma.getData();
  data[0] = 0x55555555;
  data[1] = 0x00000000;
  dma.transfer(0, 2, step);
  dma.waitDone();
  pwm.waitFifoEmpty();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  trinamic.setStealthChop(true);
  reverse = direction;
  state = 0;
  return success;
}

uint32_t MotorShield::relativeMove(int32_t increment)
{
  std::lock_guard<std::mutex> guard(mutex);
  if (increment > 0) {
    rotate(!reverse, increment);
  } else {
    rotate(reverse, -increment);
  }
  state += increment;
  return state;
}

void MotorShield::absoluteMove(uint32_t position)
{
  std::lock_guard<std::mutex> guard(mutex);
  if (position > state) {
    rotate(!reverse, position - state);
  } else {
    rotate(reverse, state - position);
  }
  state = position;
}

void MotorShield::rotate(bool direction, uint32_t count)
{
  gpio.setLevel(dir_pin, direction);
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  dma.setup();
  DmaBuffer dma_buffer(dma, dma_words);
  for (uint32_t i = 0; i < count; i++) {
    uint32_t distance = std::min(i, count - 1 - i);
    static const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> speeds =
        {{32, 1, 0x55555555}, {16, 2, 0x33333333}, {0, 16, 0x0000ffff}};
    for (const auto &speed : speeds) {
      uint32_t min_distance, dma_count, dma_data;
      std::tie(min_distance, dma_count, dma_data) = speed;
      if (distance >= min_distance) {
        for (uint32_t j = 0; j < dma_count; j++) {
          dma_buffer.push(dma_data);
          dma_buffer.push(0x00000000);
        }
        break;
      }
    }
  }
  dma_buffer.flush();
  pwm.waitFifoEmpty();
  std::this_thread::sleep_for(std::chrono::microseconds(10));
}
