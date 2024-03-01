#include "dma.h"

#include <chrono>
#include <cstring>
#include <thread>

Dma::Dma(Memory &memory, Mailbox &mailbox, size_t data_size, uint32_t dma_num) :
  memory_block(memory, mailbox, 2 * sizeof(ControlBlock) + 2 * data_size * 4),
  data_size(data_size)
{
  static const size_t dma_offset[] = {
      0x00007000, 0x00007100, 0x00007200, 0x00007300, 0x00007400, 0x00007500,
      0x00007600, 0x00007700, 0x00007800, 0x00007900, 0x00007a00, 0x00007b00,
      0x00007c00, 0x00007d00, 0x00007e00, 0x00e05000};
  registers = reinterpret_cast<Registers *>(
      memory.map(memory.periphBase() + dma_offset[dma_num], sizeof(Registers)));
  memset(registers, 0, sizeof(Registers));
  control_blocks = reinterpret_cast<ControlBlock *>(memory_block[0]);
  memset(control_blocks, 0, 2 * sizeof(ControlBlock));
  data = reinterpret_cast<uint32_t *>(memory_block[2 * sizeof(ControlBlock)]);
  state = false;
}

uint32_t *Dma::getData()
{
  return &data[state ? data_size : 0];
}

void Dma::setup()
{
  CsReg cs_reset = {.RESET = 1};
  registers->cs = cs_reset;
  std::this_thread::sleep_for(std::chrono::microseconds(10));
  CsReg cs_setup = {
      .PRIORITY = 15,
      .PANIC_PRIORITY = 15,
      .WAIT_OUTSTANDING_WRITES = 1,
  };
  registers->cs = cs_setup;
  std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void Dma::transfer(size_t offset, size_t size, size_t count)
{
  uint32_t control_block_address =
      memory_block.busAddr(createControlBlock(offset, size, count));
  registers->cs.ACTIVE = 0;
  do {
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  } while (registers->cs.PAUSED == 0 && registers->conblk_ad != 0);
  if (registers->conblk_ad == 0) {
    registers->conblk_ad = control_block_address;
  } else {
    registers->nextconbk = control_block_address;
    control_blocks[state ? 1 : 0].nextconbk = control_block_address;
  }
  registers->cs.ACTIVE = 1;
}

void Dma::waitReady()
{
  do {
    std::this_thread::sleep_for(std::chrono::microseconds(100));
  } while (registers->nextconbk);
}

void Dma::waitDone()
{
  do {
    std::this_thread::sleep_for(std::chrono::microseconds(100));
  } while (registers->conblk_ad);
}

Dma::ControlBlock *Dma::createControlBlock(size_t offset, size_t size,
                                           size_t count)
{
  ControlBlock *control_block = &control_blocks[state ? 1 : 0];
  TiReg ti = {.TDMODE = 1,
              .WAIT_RESP = 1,
              .DEST_DREQ = 1,
              .SRC_INC = 1,
              .PERMAP = 5,
              .NO_WIDE_BURSTS = 1};
  control_block->ti = ti;
  control_block->source_ad = memory_block.busAddr(getData()) + offset * 4;
  control_block->dest_ad = 0x7e20c000 + 24;
  TxfrLenReg txfr_len = {.XLENGTH = size * 4, .YLENGTH = count - 1};
  control_block->txfr_len = txfr_len;
  StrideReg stride = {.S_STRIDE = -(size * 4)};
  control_block->stride = stride;
  control_block->nextconbk = 0;
  state = !state;
  return control_block;
}
