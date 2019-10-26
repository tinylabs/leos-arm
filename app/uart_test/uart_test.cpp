/**
 *  Test UART passed in as argv[1]
 *
 *  All rights reserved.
 *  Tiny Labs Inc
 *  2019
 */
#include "leos.h"
#include <leos/iChar.h>

int main (int argc, char **argv)
{
  // Open UARTs
  iChar *uart = (iChar *)leos_load ("UART.0");
  if (uart->Setup ("mode=8N1 baud=115200"))
    return -1;

  // Write to UART
  uart->Write ("Hello UART0\n", 12);

  // Run loopback
  while (1) {
    uint8_t c[1];
    uart->Read (c, 1);
    if (c[0] == 4) // Break on control-d
      break;
    uart->Write (c, 1);
  }
  
  // Unload
  leos_unload (uart);
  return 0;
}
