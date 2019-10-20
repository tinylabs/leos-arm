/* DO NOT EDIT: This file is autogenerated. */
#ifdef HEADER_78E34888
  #error "Only include from driver CPP!"
#endif
#define HEADER_78E34888

#include <leos/reg.h>
typedef REG32 _0000; /* DATA */
typedef struct {
  union {
    REG32 REG;
    struct {
      REG32 TXBF         : 1; /* RW: TX Buffer Full */
      REG32 RXBF         : 1; /* RW: RX Buffer Full */
      REG32 TXOV         : 1; /* RW: TX Buffer Overun (write 1 to clear) */
      REG32 RXOV         : 1; /* RW: RX Buffer Overun (write 1 to clear) */
    };
  };
} _0004; /* STATE */

typedef enum {
  TXEN_Disable       = 0, /* Disabled */
  TXEN_Enable        = 1, /* Enabled */
} TXEN_enum;

typedef enum {
  RXEN_Disable       = 0, /* Disabled */
  RXEN_Enable        = 1, /* Enabled */
} RXEN_enum;

typedef enum {
  TXINT_Disable      = 0, /* Disabled */
  TXINT_Enable       = 1, /* Enabled */
} TXINT_enum;

typedef enum {
  RXINT_Disable      = 0, /* Disabled */
  RXINT_Enable       = 1, /* Enabled */
} RXINT_enum;

typedef enum {
  TXOVINT_Disable    = 0, /* Disabled */
  TXOVINT_Enable     = 1, /* Enabled */
} TXOVINT_enum;

typedef enum {
  RVOVINT_Disable    = 0, /* Disabled */
  RVOVINT_Enable     = 1, /* Enabled */
} RVOVINT_enum;

typedef enum {
  HSTX_Disable       = 0, /* Disabled */
  HSTX_Enable        = 1, /* Enabled */
} HSTX_enum;

typedef struct {
  union {
    REG32 REG;
    struct {
      REG32 TXEN         : 1; /* RW: TX Enable */
      REG32 RXEN         : 1; /* RW: RX Enable */
      REG32 TXINT        : 1; /* RW: TX Interrupt Enable */
      REG32 RXINT        : 1; /* RW: RX Interrupt Enable */
      REG32 TXOVINT      : 1; /* RW: TX Overrun Interrupt Enable */
      REG32 RVOVINT      : 1; /* RW: RX Overrun Interrupt Enable */
      REG32 HSTX         : 1; /* RW: High Speed Test Mode for TX only */
    };
  };
} _0008; /* CTRL */

typedef struct {
  union {
    REG32 REG;
    struct {
      REG32 TXINT        : 1; /* RO: TX interrupt */
      REG32 RXINT        : 1; /* RO: RX interrupt */
      REG32 TXOV         : 1; /* RO: TX Overrun interrupt */
      REG32 RXOV         : 1; /* RO: RX Overrun interrupt */
    };
  };
} _000C; /* INTSTATUS */

typedef REG32 _0010; /* BAUDDIV */

// Top-level register defs
typedef struct {
  _0000 DATA;          /* RW: Recieve and Transmit Data Value */
  _0004 STATE;         /* RW: UART Status Register */
  _0008 CTRL;          /* RW: UART Control Register */
  union {
    _000C INTSTATUS;     /* RO: UART Interrupt Status Register */
    _000C INTCLEAR;      /* WO: UART Interrupt CLEAR Register */
  };
  _0010 BAUDDIV;       /* RW: Baudrate Divider */
} reg_t;
#define P78E34888_FP 0x00000000
#define P78E34888_SZ 20

