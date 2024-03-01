#pragma once

#include "memory.h"

#include <cstdbool>
#include <cstdint>

class Gpio
{
public:
  Gpio(Memory &memory);
  void setLevel(uint8_t pin, bool level);
  void setOutput(uint8_t pin, bool output);
  void setFunction(uint8_t pin, uint8_t function);

private:
  struct Registers
  {
    volatile uint32_t fsel[6]; // GPIO Function Select
    volatile uint32_t resvd_0x18;
    volatile uint32_t set[2]; // GPIO Pin Output Set
    volatile uint32_t resvd_0x24;
    volatile uint32_t clr[2]; // GPIO Pin Output Clear
    volatile uint32_t resvd_0x30;
    volatile uint32_t lev[2]; // GPIO Pin Level
    volatile uint32_t resvd_0x3c;
    volatile uint32_t eds[2]; // GPIO Pin Event Detect Status
    volatile uint32_t resvd_0x48;
    volatile uint32_t ren[2]; // GPIO Pin Rising Edge Detect Enable
    volatile uint32_t resvd_0x54;
    volatile uint32_t fen[2]; // GPIO Pin Falling Edge Detect Enable
    volatile uint32_t resvd_0x60;
    volatile uint32_t hen[2]; // GPIO Pin High Detect Enable
    volatile uint32_t resvd_0x6c;
    volatile uint32_t len[2]; // GPIO Pin Low Detect Enable
    volatile uint32_t resvd_0x78;
    volatile uint32_t aren[2]; // GPIO Pin Async Rising Edge Detect
    volatile uint32_t resvd_0x84;
    volatile uint32_t afen[2]; // GPIO Pin Async Falling Edge Detect
    volatile uint32_t resvd_0x90;
    volatile uint32_t pud; // GPIO Pin Pull up/down Enable
    volatile uint32_t pudclk[2]; // GPIO Pin Pull up/down Enable Clock
    volatile uint32_t resvd_0xa0[4];
    volatile uint32_t test;
  } __attribute__((packed, aligned(4)));
  Registers *registers;
};
