#include "gpio.h"

Gpio::Gpio(Memory &memory)
{
  static const size_t gpio_offset = 0x00200000;
  registers = reinterpret_cast<Registers *>(
      memory.map(memory.periphBase() + gpio_offset, sizeof(Registers)));
}

void Gpio::setLevel(uint8_t pin, bool level)
{
  int regnum = pin >> 5;
  int offset = (pin & 0x1f);
  if (level) {
    registers->set[regnum] = (1 << offset);
  } else {
    registers->clr[regnum] = (1 << offset);
  }
}

void Gpio::setOutput(uint8_t pin, bool output)
{
  int regnum = pin / 10;
  int offset = (pin % 10) * 3;
  registers->fsel[regnum] &= ~(0x7 << offset);
  if (output) {
    registers->fsel[regnum] |= 0x1 << offset;
  }
}

void Gpio::setFunction(uint8_t pin, uint8_t function)
{
  int regnum = pin / 10;
  int offset = (pin % 10) * 3;
  static const uint32_t funcmap[] = {4, 5, 6, 7, 3, 2};
  registers->fsel[regnum] &= ~(0x7 << offset);
  if (function < 6) {
    registers->fsel[regnum] |= funcmap[function] << offset;
  }
}
