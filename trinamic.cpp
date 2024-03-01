#include "trinamic.h"

#include <exception>

Trinamic::Trinamic(const std::string &path) : spi(path) {}

void Trinamic::setup()
{
  Reg reg;
  reg.value = 0;
  reg.gconf.en_pwm_mode = 1;
  writeRegister(gconf_reg, reg.value);
  reg.value = 0;
  reg.ihold_irun.ihold = 0;
  reg.ihold_irun.irun = 8;
  reg.ihold_irun.iholddelay = 1;
  writeRegister(ihold_irun_reg, reg.value);
  reg.value = 0;
  reg.tpowerdown.tpowerdown = 1;
  writeRegister(tpowerdown_reg, reg.value);
  reg.value = 0;
  reg.chopconf.toff = 2;
  reg.chopconf.tbl = 1;
  reg.chopconf.vsense = 1;
  writeRegister(chopconf_reg, reg.value);
}

void Trinamic::disable()
{
  Reg reg;
  reg.value = 0;
  reg.gconf.direct_mode = 1;
  writeRegister(gconf_reg, reg.value);
}

uint32_t Trinamic::getCounter()
{
  Reg reg;
  reg.value = readRegister(mscnt_reg);
  return reg.mscnt.mscnt;
}

uint32_t Trinamic::getLoad()
{
  Reg reg;
  reg.value = readRegister(drv_status_reg);
  return reg.drv_status.sg_result;
}

uint32_t Trinamic::getPwmScale()
{
  Reg reg;
  reg.value = readRegister(pwm_scale_reg);
  return reg.pwm_scale.pwm_scale;
}

void Trinamic::setStealthChop(bool stealth_chop)
{
  Reg reg;
  reg.value = readRegister(gconf_reg);
  if (stealth_chop) {
    reg.gconf.en_pwm_mode = 1;
  } else {
    reg.gconf.en_pwm_mode = 0;
  }
  writeRegister(gconf_reg, reg.value);
}

void Trinamic::setDoubleEdge(bool double_edge)
{
  Reg reg;
  reg.value = readRegister(chopconf_reg);
  if (double_edge) {
    reg.chopconf.dedge = 1;
  } else {
    reg.chopconf.dedge = 0;
  }
  writeRegister(chopconf_reg, reg.value);
}

void Trinamic::setMicrosteps(uint32_t microsteps)
{
  Reg reg;
  reg.value = readRegister(chopconf_reg);
  switch (microsteps) {
    case 1:
      reg.chopconf.mres = 8;
      break;
    case 2:
      reg.chopconf.mres = 7;
      break;
    case 4:
      reg.chopconf.mres = 6;
      break;
    case 8:
      reg.chopconf.mres = 5;
      break;
    case 16:
      reg.chopconf.mres = 4;
      break;
    case 32:
      reg.chopconf.mres = 3;
      break;
    case 64:
      reg.chopconf.mres = 2;
      break;
    case 128:
      reg.chopconf.mres = 1;
      break;
    case 256:
      reg.chopconf.mres = 0;
      break;
    default:
      throw std::exception();
  }
  writeRegister(chopconf_reg, reg.value);
}

uint32_t Trinamic::readRegister(uint8_t address)
{
  uint8_t data_0[5] = {address};
  spi.transfer(data_0, sizeof(data_0));
  uint8_t data_1[5] = {};
  spi.transfer(data_1, sizeof(data_1));
  uint32_t *data_value = reinterpret_cast<uint32_t *>(&data_1[1]);
  return __builtin_bswap32(*data_value);
}

void Trinamic::writeRegister(uint8_t address, uint32_t value)
{
  uint8_t write_address = address | 0x80;
  uint8_t data[5] = {write_address};
  uint32_t *data_value = reinterpret_cast<uint32_t *>(&data[1]);
  *data_value = __builtin_bswap32(value);
  spi.transfer(data, sizeof(data));
}
