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
  // Open UART
  iChar *uart = (iChar *)leos_load ("UART.0");

  // Setup UART
  uart->Setup ("mode=8N1 baud=115200");

  // Write to UART
  uart->Write ("Hello World", 12);

  // Unload
  leos_unload (uart);
  return 0;
}
