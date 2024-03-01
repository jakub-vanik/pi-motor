#include "spi.h"

#include <cstring>
#include <exception>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>

Spi::Spi(const std::string &path) : path(path)
{
  fd = open(path.c_str(), O_RDWR);
  if (fd == -1) {
    throw std::exception();
  }
  mode = 0;
  bits = 8;
  speed = 500000;
  if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
    throw std::exception();
  }
  if (ioctl(fd, SPI_IOC_RD_MODE, &mode) == -1) {
    throw std::exception();
  }
  if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1) {
    throw std::exception();
  }
  if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1) {
    throw std::exception();
  }
  if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
    throw std::exception();
  }
  if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1) {
    throw std::exception();
  }
}

Spi::~Spi()
{
  close(fd);
}

void Spi::transfer(uint8_t *data, uint32_t size)
{
  uint8_t tx[size];
  uint8_t rx[size];
  memcpy(tx, data, size);
  struct spi_ioc_transfer tr = {
      .tx_buf = (unsigned long) tx,
      .rx_buf = (unsigned long) rx,
      .len = size,
      .speed_hz = speed,
      .delay_usecs = 0,
      .bits_per_word = bits
  };
  if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) == -1) {
    throw std::exception();
  }
  memcpy(data, rx, size);
}
