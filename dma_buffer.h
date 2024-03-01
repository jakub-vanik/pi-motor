#pragma once

#include "dma.h"

#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <vector>

class DmaBuffer
{
public:
  DmaBuffer(Dma &dma, size_t size);
  void push(uint32_t data);
  void flush();

private:
  Dma &dma;
  size_t size;
  bool state;
  std::vector<uint32_t> buffer;
  void transfer();
};
