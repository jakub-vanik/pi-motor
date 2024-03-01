#include "pwm.h"

#include <chrono>
#include <thread>

Pwm::Pwm(Memory &memory)
{
  static const size_t pwm_offset = 0x0020c000;
  registers = reinterpret_cast<Registers *>(
      memory.map(memory.periphBase() + pwm_offset, sizeof(Registers)));
}

void Pwm::startSerial()
{
  clearFifo();
  registers->rng1 = 32;
  registers->rng2 = 32;
  setupDma();
  CtlReg ctl = {};
  ctl.MODE1 = 1;
  ctl.USEF1 = 1;
  ctl.MODE2 = 1;
  ctl.USEF2 = 1;
  registers->ctl = ctl;
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  ctl.PWEN1 = 1;
  ctl.PWEN2 = 1;
  registers->ctl = ctl;
  std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void Pwm::waitFifoEmpty()
{
  do {
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  } while (!registers->sta.EMPT1);
}

void Pwm::stop()
{
  CtlReg ctl = {};
  registers->ctl = ctl;
  std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void Pwm::clearFifo()
{
  CtlReg ctl = {};
  ctl.CLRF1 = 1;
  registers->ctl = ctl;
  std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void Pwm::setupDma()
{
  DmacReg dmac = {};
  dmac.ENAB = 1;
  dmac.PANIC = 7;
  dmac.DREQ = 3;
  registers->dmac = dmac;
  std::this_thread::sleep_for(std::chrono::microseconds(10));
}