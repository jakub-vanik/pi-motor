#include "memory.h"

#include <exception>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

Memory::Memory()
{
  fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd == -1) {
    throw std::exception();
  }
}

Memory::~Memory()
{
  close(fd);
}

size_t Memory::periphBase()
{
  return 0x3f000000;
}

size_t Memory::busToPhys(size_t address)
{
  return address & ~0xC0000000;
}

void *Memory::map(size_t address, size_t size)
{
  static const size_t page_mask = ~0UL ^ (getpagesize() - 1);
  static const size_t offset_mask = getpagesize() - 1;
  size_t offset = address & offset_mask;
  size_t map_length = offset + size;
  size_t map_offset = address & page_mask;
  void *page =
      mmap(0, map_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, map_offset);
  if (page == MAP_FAILED) {
    throw std::exception();
  }
  return reinterpret_cast<char *>(page) + offset;
}
