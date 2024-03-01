#pragma once

#include "spi.h"

#include <cstdbool>
#include <cstdint>
#include <string>

class Trinamic
{
public:
  Trinamic(const std::string &path);
  void setup();
  void disable();
  uint32_t getCounter();
  uint32_t getLoad();
  uint32_t getPwmScale();
  void setStealthChop(bool stealth_chop);
  void setDoubleEdge(bool double_edge);
  void setMicrosteps(uint32_t microsteps);

private:
  static const uint8_t gconf_reg = 0x00;
  static const uint8_t ihold_irun_reg = 0x10;
  static const uint8_t tpowerdown_reg = 0x11;
  static const uint8_t mscnt_reg = 0x6a;
  static const uint8_t chopconf_reg = 0x6c;
  static const uint8_t drv_status_reg = 0x6f;
  static const uint8_t pwm_scale_reg = 0x71;
  struct GconfReg
  {
    uint32_t i_scale_analog : 1;
    uint32_t nternal_rsense : 1;
    uint32_t en_pwm_mode : 1;
    uint32_t enc_commutation : 1;
    uint32_t shaft : 1;
    uint32_t diag0_error : 1;
    uint32_t diag0_otpw : 1;
    uint32_t diag0_stall : 1;
    uint32_t diag1_stall : 1;
    uint32_t diag1_index : 1;
    uint32_t diag1_onstate : 1;
    uint32_t diag1_steps_skipped : 1;
    uint32_t diag0_int_pushpull : 1;
    uint32_t diag1_pushpull : 1;
    uint32_t small_hysteresis : 1;
    uint32_t stop_enable : 1;
    uint32_t direct_mode : 1;
    uint32_t test_mode : 1;
  } __attribute__((packed));
  struct IholdIrunReg
  {
    uint32_t ihold : 4;
    uint32_t res_5 : 4;
    uint32_t irun : 4;
    uint32_t res_13 : 4;
    uint32_t iholddelay : 4;
  } __attribute__((packed));
  struct TpowerdownReg
  {
    uint32_t tpowerdown : 8;
  } __attribute__((packed));
  struct MscntReg
  {
    uint32_t mscnt : 10;
  } __attribute__((packed));
  struct ChopconfReg
  {
    uint32_t toff : 4;
    uint32_t hstrt : 3;
    uint32_t hend : 4;
    uint32_t fd3 : 1;
    uint32_t disfdcc : 1;
    uint32_t rndtf : 1;
    uint32_t chm : 1;
    uint32_t tbl : 2;
    uint32_t vsense : 1;
    uint32_t vhighfs : 1;
    uint32_t vhighchm : 1;
    uint32_t sync : 4;
    uint32_t mres : 4;
    uint32_t intpol : 1;
    uint32_t dedge : 1;
    uint32_t diss2g : 1;
  } __attribute__((packed));
  struct DrvStatusReg
  {
    uint32_t sg_result : 10;
    uint32_t res_10 : 5;
    uint32_t fsactive : 1;
    uint32_t cs_actual : 5;
    uint32_t res_21 : 3;
    uint32_t stallguard : 1;
    uint32_t ot : 1;
    uint32_t otpw : 1;
    uint32_t s2ga : 1;
    uint32_t s2gb : 1;
    uint32_t ola : 1;
    uint32_t olb : 1;
    uint32_t stst : 1;
  } __attribute__((packed));
  struct PwmScaleReg
  {
    uint32_t pwm_scale : 8;
  } __attribute__((packed));
  union Reg {
    GconfReg gconf;
    IholdIrunReg ihold_irun;
    TpowerdownReg tpowerdown;
    MscntReg mscnt;
    ChopconfReg chopconf;
    DrvStatusReg drv_status;
    PwmScaleReg pwm_scale;
    uint32_t value;
  };
  Spi spi;
  uint32_t readRegister(uint8_t address);
  void writeRegister(uint8_t address, uint32_t value);
};
