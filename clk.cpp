#include "clk.h"

#include <chrono>
#include <thread>

Clk::Clk(Memory &memory)
{
  static const size_t clk_offset = 0x001010a0;
  registers = reinterpret_cast<Registers *>(
      memory.map(memory.periphBase() + clk_offset, sizeof(Registers)));
}

void Clk::start(uint32_t freq)
{
  static const uint64_t base_freq = 19200000ULL * 1024;
  uint32_t divider = base_freq / freq;
  DivReg div = {};
  div.PASSWD = 0x5a;
  div.DIVI = divider / 1024;
  div.DIVF = divider % 1024;
  registers->div = div;
  CtlReg ctl = {};
  ctl.PASSWD = 0x5a;
  ctl.SRC = 1;
  ctl.MASH = 1;
  registers->ctl = ctl;
  ctl.ENAB = 1;
  registers->ctl = ctl;
  do {
    std::this_thread::sleep_for(std::chrono::microseconds(100));
  } while (!registers->ctl.BUSY);
}

void Clk::stop()
{
  CtlReg ctl = {};
  ctl.PASSWD = 0x5a;
  ctl.KILL = 1;
  registers->ctl = ctl;
  do {
    std::this_thread::sleep_for(std::chrono::microseconds(100));
  } while (registers->ctl.BUSY);
}
