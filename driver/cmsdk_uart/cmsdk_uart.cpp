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

