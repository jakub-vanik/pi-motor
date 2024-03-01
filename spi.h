#pragma once

#include <cstdint>
#include <string>

class Spi
{
public:
  Spi(const std::string &path);
  virtual ~Spi();
  void transfer(uint8_t *data, uint32_t size);

private:
  std::string path;
  int fd;
  uint8_t mode;
  uint8_t bits;
  uint32_t speed;
};
