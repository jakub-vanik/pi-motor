#pragma once

#include "mailbox.h"
#include "memory.h"

#include <cstdint>

class MemoryBlock
{
public:
  MemoryBlock(Memory &memory, Mailbox &mailbox, uint32_t size);
  virtual ~MemoryBlock();
  uint32_t busAddr();
  uint32_t busAddr(const void *addr);
  void *operator*() const;
  void *operator[](uint32_t index) const;

private:
  Mailbox &mailbox;
  uint32_t handle;
  uint32_t bus_addr;
  void *pointer;
};
