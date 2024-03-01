#pragma once

#include <cstddef>

class Memory
{
public:
  Memory();
  virtual ~Memory();
  size_t periphBase();
  size_t busToPhys(size_t address);
  void *map(size_t address, size_t size);

private:
  int fd;
};
