#pragma once

#include "memory.h"

#include <cstdint>

class Clk
{
public:
  Clk(Memory &memory);
  void start(uint32_t freq);
  void stop();

private:
  struct CtlReg
  {
    volatile uint32_t SRC : 4;
    volatile uint32_t ENAB : 1;
    volatile uint32_t KILL : 1;
    volatile uint32_t RES_6 : 1;
    volatile uint32_t BUSY : 1;
    volatile uint32_t FLIP : 1;
    volatile uint32_t MASH : 2;
    volatile uint32_t RES_11 : 13;
    volatile uint32_t PASSWD : 8;
  } __attribute__((packed));
  struct DivReg
  {
    volatile uint32_t DIVF : 12;
    volatile uint32_t DIVI : 12;
    volatile uint32_t PASSWD : 8;
  } __attribute__((packed));
  struct Registers
  {
    CtlReg ctl;
    DivReg div;
  } __attribute__((packed, aligned(4)));
  Registers *registers;
};
