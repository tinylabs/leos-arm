/**
 *  ARM MPS2(+) development platform system controller
 *
 *  All rights reserved.
 *  Tiny Labs Inc
 *  2019
 */

#include "leos.h"
#include <leos/iSysctl.h>
#include "header_a7c12a92.h"

class cmsdk_sysctl : public iSysctl {
 private:
  reg_t *reg;
  uint32_t oscclk[3];
  
 public:
  cmsdk_sysctl (int idx, int cnt, va_list ap);
  ~cmsdk_sysctl () {}

  // Common functions
  int Setup (const char *args);
  void Cleanup (void);
  
  // iSysctl interface
  uint32_t ClkGet (hwnode_t node);
  int ClkSet (hwnode_t node, uint32_t val);
  int Reset (hwnode_t node);
  int ResetReason (void);
};

EXPORT_OBJ (cmsdk_sysctl, SINGLETON);


cmsdk_sysctl::cmsdk_sysctl (int idx, int cnt, va_list ap)
  : iSysctl (idx)
{
  // Ignore clk node
  va_arg (ap, char *);

  // Get register base
  reg = (reg_t *)va_arg (ap, uint32_t);
}

int cmsdk_sysctl::Setup (const char *args)
{
  int rv;
  uint32_t val;

  // Make system call to map clock tree
  rv = leos_clock_map ("/cfg/mps2_clock.bcf");
  if (rv)
    return rv;

  // Set root clks if specified
  rv = leos_parse_uint (args, "OSCCLK0=", &val);
  if (!rv)
    ClkSet (C_OSCCLK0, val);
  rv = leos_parse_uint (args, "OSCCLK1=", &val);
  if (!rv)
    ClkSet (C_OSCCLK1, val);
  rv = leos_parse_uint (args, "OSCCLK2=", &val);
  if (!rv)
    ClkSet (C_OSCCLK2, val);

  // Success
  return 0;
}

void cmsdk_sysctl::Cleanup (void)
{

}

uint32_t cmsdk_sysctl::ClkGet (hwnode_t node)
{
  switch (node) {
    case C_OSCCLK0:
      return oscclk[0];
      
    case C_OSCCLK1:
      return oscclk[1];

    case C_OSCCLK2:
      return oscclk[2];

    // These are fixed prescalers set to 2
    case C_SYSCLK:
    case C_DBGCLK:
    case C_SPICLCD:
    case C_SPICON:
    case C_I2CCLCD:
    case C_I2CAUD:
    case C_AUDMCLK:
      return 2;

    // Fixed prescaler of 8
    case C_AUDSCLK:
      return 8;

    // These are all gates and always enabled - return 1
    case C_TIMER0:
    case C_TIMER1:
    case C_DUALTIMER:
    case C_UART0:
    case C_UART1:
    case C_UART2:
    case C_UART3:
    case C_UART4:
    case C_WDT:
    case C_GPIO0:
    case C_GPIO1:
    case C_SPI:
      return 1;

    // Unknown or NULL clock return 0
    case C_NULL:
    default:
      return 0;
  }
}

int cmsdk_sysctl::ClkSet (hwnode_t node, uint32_t val)
{
  switch (node) {
    // These root nodes depend on external inputs
    // Must be set at boot
    case C_OSCCLK0:
      oscclk[0] = val;
      break;
      
    case C_OSCCLK1:
      oscclk[1] = val;
      break;

    case C_OSCCLK2:
      oscclk[2] = val;
      break;

    // These are all fixed and cannot be set
    case C_SYSCLK:
    case C_DBGCLK:
    case C_SPICLCD:
    case C_SPICON:
    case C_I2CCLCD:
    case C_I2CAUD:
    case C_AUDMCLK:
    case C_AUDSCLK:
    case C_TIMER0:
    case C_TIMER1:
    case C_DUALTIMER:
    case C_UART0:
    case C_UART1:
    case C_UART2:
    case C_UART3:
    case C_UART4:
    case C_WDT:
    case C_GPIO0:
    case C_GPIO1:
    case C_SPI:
    case C_NULL:
    default:
      return -1;
  }

  // Success
  return 0;
}

int cmsdk_sysctl::Reset (hwnode_t node)
{
  // Cannot reset any hardware
  return -1;
}

int cmsdk_sysctl::ResetReason (void)
{
  // Reset reason unknown
  return -1;
}

