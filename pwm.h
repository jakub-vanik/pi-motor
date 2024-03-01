#pragma once

#include "memory.h"

#include <cstdint>

class Pwm
{
public:
  Pwm(Memory &memory);
  void startSerial();
  void waitFifoEmpty();
  void stop();

private:
  struct CtlReg
  {
    volatile uint32_t PWEN1 : 1;
    volatile uint32_t MODE1 : 1;
    volatile uint32_t RPTL1 : 1;
    volatile uint32_t SBIT1 : 1;
    volatile uint32_t POLA1 : 1;
    volatile uint32_t USEF1 : 1;
    volatile uint32_t CLRF1 : 1;
    volatile uint32_t MSEN1 : 1;
    volatile uint32_t PWEN2 : 1;
    volatile uint32_t MODE2 : 1;
    volatile uint32_t RPTL2 : 1;
    volatile uint32_t SBIT2 : 1;
    volatile uint32_t POLA2 : 1;
    volatile uint32_t USEF2 : 1;
    volatile uint32_t RES_B14 : 1;
    volatile uint32_t MSEN2 : 1;
    volatile uint32_t RES_B16 : 16;
  } __attribute__((packed));
  struct StaReg
  {
    volatile uint32_t FULL1 : 1;
    volatile uint32_t EMPT1 : 1;
    volatile uint32_t WERR1 : 1;
    volatile uint32_t RERR1 : 1;
    volatile uint32_t GAP01 : 1;
    volatile uint32_t GAP02 : 1;
    volatile uint32_t GAP03 : 1;
    volatile uint32_t GAP04 : 1;
    volatile uint32_t BERR : 1;
    volatile uint32_t STA1 : 1;
    volatile uint32_t STA2 : 1;
    volatile uint32_t STA3 : 1;
    volatile uint32_t STA4 : 1;
    volatile uint32_t RES_B13 : 19;
  } __attribute__((packed));
  struct DmacReg
  {
    volatile uint32_t DREQ : 8;
    volatile uint32_t PANIC : 8;
    volatile uint32_t RES_B16 : 15;
    volatile uint32_t ENAB : 1;
  } __attribute__((packed));
  struct Registers
  {
    CtlReg ctl;
    StaReg sta;
    DmacReg dmac;
    volatile uint32_t resvd_0x0c;
    volatile uint32_t rng1;
    volatile uint32_t dat1;
    volatile uint32_t fif1;
    volatile uint32_t resvd_0x1c;
    volatile uint32_t rng2;
    volatile uint32_t dat2;
  } __attribute__((packed, aligned(4)));
  Registers *registers;
  void clearFifo();
  void setupDma();
};
