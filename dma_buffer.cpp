#include "dma_buffer.h"

#include <cstring>

DmaBuffer::DmaBuffer(Dma &dma, size_t size) : dma(dma), size(size)
{
  state = false;
  buffer.reserve(size / 2);
}

void DmaBuffer::push(uint32_t data)
{
  buffer.push_back(data);
  if (buffer.size() == size / 2) {
    transfer();
  }
}

void DmaBuffer::flush()
{
  if (!buffer.empty()) {
    transfer();
  }
  dma.waitDone();
}

void DmaBuffer::transfer()
{
  size_t offset = state ? size / 2 : 0;
  size_t size = buffer.size();
  memcpy(dma.getData() + offset, buffer.data(), size * 4);
  dma.waitReady();
  dma.transfer(offset, size, 1);
  state = !state;
  buffer.clear();
}
