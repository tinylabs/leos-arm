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
  rv = leos_clock_map ("/cfg/mps2_map.bcf");
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
      break;
      
    case C_OSCCLK1:
      return oscclk[1];
      break;

    case C_OSCCLK2:
      return oscclk[2];
      break;

    default:
      return 0;
  }
}

int cmsdk_sysctl::ClkSet (hwnode_t node, uint32_t val)
{
  switch (node) {
    case C_OSCCLK0:
      oscclk[0] = val;
      break;
      
    case C_OSCCLK1:
      oscclk[1] = val;
      break;

    case C_OSCCLK2:
      oscclk[2] = val;
      break;

    default:
      return -1;
  }
  return 0;
}

int cmsdk_sysctl::Reset (hwnode_t node)
{
  return 0;
}

int cmsdk_sysctl::ResetReason (void)
{
  return -1;
}

