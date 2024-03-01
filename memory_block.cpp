#include "memory_block.h"

#include <unistd.h>

MemoryBlock::MemoryBlock(Memory &memory, Mailbox &mailbox, uint32_t size) :
  mailbox(mailbox)
{
  handle = mailbox.memAlloc(size, getpagesize(), 0x4);
  bus_addr = mailbox.memLock(handle);
  pointer = memory.map(memory.busToPhys(bus_addr), size);
}

MemoryBlock::~MemoryBlock()
{
  mailbox.memUnlock(handle);
  mailbox.memFree(handle);
}

uint32_t MemoryBlock::busAddr()
{
  return bus_addr;
}

uint32_t MemoryBlock::busAddr(const void *addr)
{
  uint32_t offset = reinterpret_cast<const char *>(addr) -
                    reinterpret_cast<const char *>(pointer);
  return bus_addr + offset;
}

void *MemoryBlock::operator*() const
{
  return pointer;
}

void *MemoryBlock::operator[](uint32_t index) const
{
  return reinterpret_cast<char *>(pointer) + index;
}
