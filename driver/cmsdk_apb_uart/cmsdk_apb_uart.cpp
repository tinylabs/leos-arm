/**
 *  CMSDK APB UART driver - Used on ARM MPS2/3 dev boards
 *
 *  All rights reserved.
 *  Tiny Labs Inc
 *  2019
 */

#include "header_78e34888.h"
#include <leos/iChar.h>
#include "leos.h"

class cmsdk_apb_uart : public iChar {

 private:
  reg_t *reg;
  clk_node_t clk;
  ISR_TRAMP rx_trmp, tx_trmp;
  uint8_t rx_irq, tx_irq;
  
public:
  cmsdk_apb_uart (int idx, int cnt, va_list ap);
  ~cmsdk_apb_uart () {}

  // ISRs
  void TxISR (void);
  void RxISR (void);
  
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

  // Init tramps
  ISR_INIT_TRAMP (&tx_trmp, &cmsdk_apb_uart::TxISR);
  ISR_INIT_TRAMP (&rx_trmp, &cmsdk_apb_uart::RxISR);

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
  uint32_t baudrate;
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
  leos_free (mode);
  if (rv)
    return -1;

  // Init rxbuf/txbuf
  // Get baudrate
  rv = leos_parse_uint (args, "baud=", &baudrate);
  if (rv || !baudrate)
    return -1;

  // Install IRQs
  leos_irq_isr (tx_irq, &tx_trmp);
  leos_irq_isr (rx_irq, &rx_trmp);
  leos_irq_pri (tx_irq, 1);
  leos_irq_pri (rx_irq, 1);
    
  // Setup divisor
  reg->BAUDDIV = leos_clock_freq (clk) / baudrate;

  // Enable interrupts
  reg->CTRL.RXINT = 1;
  reg->CTRL.TXINT = 1;
  
  // Enable UART
  reg->CTRL.RXEN = 1;
  reg->CTRL.TXEN = 1;
  return 0;
}

void cmsdk_apb_uart::Cleanup (void)
{
  // Disable UART
  reg->CTRL.TXEN = 0;
  reg->CTRL.RXEN = 0;
  
  // Disable interrupts
  reg->CTRL.RXINT = 0;
  reg->CTRL.TXINT = 0;

  // Uninstall interrupt routines
  leos_irq_isr (tx_irq, NULL);
  leos_irq_isr (rx_irq, NULL);
}

int cmsdk_apb_uart::Read (void *buf, int len)
{
  int i;
  char *cbuf = ( char *)buf;

  for (i = 0; i < len; i++) {
    // Wait for transmitter to be ready
    while (reg->STATE.RXBF == 0)
      ;
    // Read next character
    cbuf[i] = reg->DATA;
  }
  
  return len;
}

int cmsdk_apb_uart::Write (const void *buf, int len)
{
  int i;
  const char *cbuf = (const char *)buf;
  for (i = 0; i < len; i++) {
    // Wait for transmitter to be ready
    while (reg->STATE.TXBF)
      ;
    // Send next character
    reg->DATA = cbuf[i];
  }
  
  return len;
}

// RX IRQ handler
void cmsdk_apb_uart::RxISR (void)
{

}

// TX IRQ handler
void cmsdk_apb_uart::TxISR (void)
{

}

