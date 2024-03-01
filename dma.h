#pragma once

#include "mailbox.h"
#include "memory.h"
#include "memory_block.h"

#include <cstdbool>
#include <cstddef>
#include <cstdint>

class Dma
{
public:
  Dma(Memory &memory, Mailbox &mailbox, size_t data_size, uint32_t dma_num);
  uint32_t *getData();
  void setup();
  void transfer(size_t offset, size_t size, size_t count);
  void waitReady();
  void waitDone();

private:
  struct CsReg
  {
    volatile uint32_t ACTIVE : 1;
    volatile uint32_t END : 1;
    volatile uint32_t INT : 1;
    volatile uint32_t DREQ : 1;
    volatile uint32_t PAUSED : 1;
    volatile uint32_t DREQ_STOPS_DMA : 1;
    volatile uint32_t WAITING_OUTSTANDING_WRITES : 1;
    volatile uint32_t RES_7 : 1;
    volatile uint32_t ERROR : 1;
    volatile uint32_t RES_9 : 7;
    volatile uint32_t PRIORITY : 4;
    volatile uint32_t PANIC_PRIORITY : 4;
    volatile uint32_t RES_24 : 4;
    volatile uint32_t WAIT_OUTSTANDING_WRITES : 1;
    volatile uint32_t DISDEBUG : 1;
    volatile uint32_t ABORT : 1;
    volatile uint32_t RESET : 1;
  } __attribute__((packed));
  struct TiReg
  {
    volatile uint32_t INTEN : 1;
    volatile uint32_t TDMODE : 1;
    volatile uint32_t RES_2 : 1;
    volatile uint32_t WAIT_RESP : 1;
    volatile uint32_t DEST_INC : 1;
    volatile uint32_t DEST_WIDTH : 1;
    volatile uint32_t DEST_DREQ : 1;
    volatile uint32_t DEST_IGNORE : 1;
    volatile uint32_t SRC_INC : 1;
    volatile uint32_t SRC_WIDTH : 1;
    volatile uint32_t SRC_DREQ : 1;
    volatile uint32_t SRC_IGNORE : 1;
    volatile uint32_t BURST_LENGTH : 4;
    volatile uint32_t PERMAP : 5;
    volatile uint32_t WAITS : 5;
    volatile uint32_t NO_WIDE_BURSTS : 1;
  } __attribute__((packed));
  struct TxfrLenReg
  {
    volatile uint32_t XLENGTH : 16;
    volatile uint32_t YLENGTH : 16;
  } __attribute__((packed));
  struct StrideReg
  {
    volatile uint32_t S_STRIDE : 16;
    volatile uint32_t D_STRIDE : 16;
  } __attribute__((packed));
  struct Registers
  {
    CsReg cs;
    volatile uint32_t conblk_ad;
    TiReg ti;
    volatile uint32_t source_ad;
    volatile uint32_t dest_ad;
    TxfrLenReg txfr_len;
    StrideReg stride;
    volatile uint32_t nextconbk;
    volatile uint32_t debug;
  } __attribute__((packed, aligned(4)));
  struct ControlBlock
  {
    TiReg ti;
    volatile uint32_t source_ad;
    volatile uint32_t dest_ad;
    TxfrLenReg txfr_len;
    StrideReg stride;
    volatile uint32_t nextconbk;
    volatile uint32_t resvd[2];
  } __attribute__((packed, aligned(4)));
  MemoryBlock memory_block;
  size_t data_size;
  Registers *registers;
  ControlBlock *control_blocks;
  uint32_t *data;
  bool state;
  ControlBlock *createControlBlock(size_t offset, size_t size, size_t count);
};
