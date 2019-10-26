/**
 *  CMSDK APB UART driver - Used on ARM MPS2/3 dev boards
 *
 *  All rights reserved.
 *  Tiny Labs Inc
 *  2019
 */

#include "header_78e34888.h"
#include <leos/iChar.h>
#include <leos/FIFO.h>
#include "leos.h"

#define DEFAULT_RXBUF_SZ  32
#define DEFAULT_TXBUF_SZ  32

class cmsdk_apb_uart : public iChar {

 private:
  CPP_ISR(tx_isr, cmsdk_apb_uart::TxISR);
  CPP_ISR(rx_isr, cmsdk_apb_uart::RxISR);
  FIFO *rxFIFO, *txFIFO;
  reg_t *reg;
  clk_node_t clk;
  uint8_t rx_irq, tx_irq;
  
  // ISRs
  void TxISR (void);
  void RxISR (void);
  
public:
  cmsdk_apb_uart (int idx, int cnt, va_list ap);
  ~cmsdk_apb_uart () {}

  // Common functions
  int Setup (const char *args);
  void Cleanup (void);

  // iChar functions
  int Read (void *buf, int len);
  int Write (const void *buf, int len);
};

EXPORT_OBJ (cmsdk_apb_uart, NORMAL);

cmsdk_apb_uart::cmsdk_apb_uart (int idx, int cnt, va_list ap)
  : iChar (idx)
{
  char *irqs;
  int rv;
  
  // Count should be at least 3
  if (cnt < 3)
    return;

  // Get name and convert to clk
  clk = k_clock_node ((char *)va_arg (ap, char *));
  
  // Get register base
  reg = (reg_t *)va_arg (ap, uint32_t);

  // Get IRQs and parse
  irqs = (char *)va_arg (ap, char *);

  // Save IRQs
  rv = k_parse_uint8 (irqs, "RX=", &rx_irq);
  rv |= k_parse_uint8 (irqs, "TX=", &tx_irq);
  if (rv)
    return;

  // Success
  valid = 1;
}

int cmsdk_apb_uart::Setup (const char *args)
{
  int rv;
  uint32_t val;
  char *mode;
  
  // Check IRQs
  if (!valid)
    return -1;

  // Parse mode
  rv = leos_parse_str (args, "mode=", &mode);
  if (rv)
    return -1;

  // Only 8N1 supported
  rv = strcmp (mode, "8N1");
  free (mode);
  if (rv)
    return -1;

  // Get baudrate
  rv = leos_parse_uint (args, "baud=", &val);
  if (rv || !val)
    return -1;

  // Setup divisor - max 20 bits
  val = leos_clock_freq (clk) / val;
  if (val > ((1 << 21) - 1))
    return -1;
  reg->BAUDDIV = val;

  // Setup RX FIFO
  rv = leos_parse_uint (args, "rxb=", &val);
  if (rv || !val)
    val = DEFAULT_RXBUF_SZ;
  rxFIFO = new FIFO (1, val);
  if (!rxFIFO)
    return -1;

  // Setup TX FIFO
  rv = leos_parse_uint (args, "txb=", &val);
  if (rv || !val)
    val = DEFAULT_TXBUF_SZ;
  txFIFO = new FIFO (1, val);
  if (!txFIFO) {
    Cleanup ();
    return -1;
  }
  
  // Install IRQs
  leos_irq_handler (tx_irq, &tx_isr);
  leos_irq_setpri (tx_irq, 1);
  leos_irq_handler (rx_irq, &rx_isr);
  leos_irq_setpri (rx_irq, 1);
  
  // Enable interrupts
  reg->CTRL.RXINT = 1;
  reg->CTRL.TXINT = 1;

  // Enable overflow interrupts
  //reg->CTRL.RXOVINT = 1;
  //reg->CTRL.TXOVINT = 1;
  
  // Enable UART
  reg->CTRL.RXEN = 1;
  reg->CTRL.TXEN = 1;
  return 0;
}

void cmsdk_apb_uart::Cleanup (void)
{
  // Free FIFOs
  if (rxFIFO)
    delete rxFIFO;
  if (txFIFO)
    delete txFIFO;

  // Disable UART
  reg->CTRL.TXEN = 0;
  reg->CTRL.RXEN = 0;
  
  // Disable interrupts
  reg->CTRL.RXINT = 0;
  reg->CTRL.TXINT = 0;
  //reg->CTRL.RXOVINT = 0;
  //reg->CTRL.TXOVINT = 0;

  // Uninstall interrupt routines
  leos_irq_handler (tx_irq, NULL);
  leos_irq_handler (rx_irq, NULL);
}

int cmsdk_apb_uart::Read (void *buf, int len)
{
  int read = 0;
  uint8_t *cbuf = (uint8_t *)buf;
  
  // Keep reading until we have data
  while (read < len) {

    // Blocking read on FIFO
    read += rxFIFO->Read (&cbuf[read], len - read);
  }
  return len;
}

int cmsdk_apb_uart::Write (const void *buf, int len)
{
  int written = 0;
  bool block = false;
  uint8_t *cbuf = (uint8_t *)buf;
  
  do {
    // Write data to FIFO don't block at first
    written += txFIFO->Write (&cbuf[written], len - written, block);
    if (!block)
      block = true;
    leos_irq_setpend (tx_irq);
  } while (written < len);

  // Return length written
  return len;
}

// RX IRQ handler
void cmsdk_apb_uart::RxISR (void)
{
  uint8_t c;
  
  // Clear interrupt
  reg->INTCLEAR.RXINT = 1;

  // Read data from hwbuf
  c = reg->DATA;
  
  // Pass to buffer
  rxFIFO->Write (&c, 1, false);
}

// TX IRQ handler
void cmsdk_apb_uart::TxISR (void)
{
  int rv;
  uint8_t c;

  // Clear interrupt
  reg->INTCLEAR.TXINT = 1;

  // Get byte and transmit
  rv = txFIFO->Read (&c, 1, false);
  if (rv == 1)
    reg->DATA = c;
}

