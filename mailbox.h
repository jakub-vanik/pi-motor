#pragma once

#include <cstdint>

class Mailbox
{
public:
  Mailbox();
  virtual ~Mailbox();
  uint32_t memAlloc(uint32_t size, uint32_t align, uint32_t flags);
  uint32_t memLock(uint32_t handle);
  uint32_t memUnlock(uint32_t handle);
  uint32_t memFree(uint32_t handle);

private:
  int fd;
  int mboxPrperty(void *param);
};
