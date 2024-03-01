#include "mailbox.h"

#include <exception>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <unistd.h>

Mailbox::Mailbox()
{
  fd = open("/dev/vcio", 0);
  if (fd == -1) {
    throw std::exception();
  }
}

Mailbox::~Mailbox()
{
  close(fd);
}

uint32_t Mailbox::memAlloc(uint32_t size, uint32_t align, uint32_t flags)
{
  uint32_t param[32] = {9 * sizeof(uint32_t),
                        0x00000000,
                        0x3000c,
                        12,
                        12,
                        size,
                        align,
                        flags,
                        0x00000000};
  if (mboxPrperty(param) < 0) {
    throw std::exception();
  }
  return param[5];
}

uint32_t Mailbox::memLock(uint32_t handle)
{
  uint32_t param[32] = {
      7 * sizeof(uint32_t), 0x00000000, 0x3000d, 4, 4, handle, 0x00000000};
  if (mboxPrperty(param) < 0) {
    throw std::exception();
  }
  return param[5];
}

uint32_t Mailbox::memUnlock(uint32_t handle)
{
  uint32_t param[32] = {
      7 * sizeof(uint32_t), 0x00000000, 0x3000e, 4, 4, handle, 0x00000000};
  if (mboxPrperty(param) < 0) {
    throw std::exception();
  }
  return param[5];
}

uint32_t Mailbox::memFree(uint32_t handle)
{
  uint32_t param[32] = {
      7 * sizeof(uint32_t), 0x00000000, 0x3000f, 4, 4, handle, 0x00000000};
  if (mboxPrperty(param) < 0) {
    throw std::exception();
  }
  return param[5];
}

int Mailbox::mboxPrperty(void *param)
{
  return ioctl(fd, _IOWR(100, 0, char *), param);
}
