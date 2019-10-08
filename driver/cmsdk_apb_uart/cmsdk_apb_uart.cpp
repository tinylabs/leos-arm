/**
 *  CMSDK UART driver - Used on ARM MPS2/3 dev boards
 *
 *  All rights reserved.
 *  Tiny Labs Inc
 *  2019
 */

#include "header_78e34888.h"
#include <leos/iChar.h>

class cmsdk_uart : public iChar {

 private:
  reg_t *reg;
  char *pname, *irqs, *pins;

public:
  cmsdk_uart (int idx, int cnt, va_list ap);
  ~cmsdk_uart () {}

  // Common functions
  int Setup (const char *args);
  void Cleanup (void);

  // iChar functions
  int Read (void *buf, int len);
  int Write (const void *buf, int len);
};

EXPORT_OBJ (cmsdk_uart, NORMAL);

cmsdk_uart::cmsdk_uart (int idx, int cnt, va_list ap)
  : iChar (idx)
{
  // Count should be at least 3
  if (cnt < 3)
    return;

  // Get name (not used)
  pname = (char *)va_arg (ap, char *);

  // Get register base
  reg = (reg_t *)va_arg (ap, uint32_t);

  // Get IRQs and parse
  irqs = (char *)va_arg (ap, char *);

  
  // Pins not used...
  //if (cnt >= 4)
  //  pins = (char *)va_arg (ap, char *);
}

int cmsdk_uart::Setup (const char *args)
{
  return 0;
}

void cmsdk_uart::Cleanup (void)
{

}

int cmsdk_uart::Read (void *buf, int len)
{
  return 0;
}

int cmsdk_uart::Write (const void *buf, int len)
{
  return 0;
}

