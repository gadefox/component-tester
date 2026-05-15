/* ************************************************************************
 *
 *   I2C bit-bang TWI
 *
 *   (c) 2017-2025 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/*
 *  include header files
 */

#include "common.h"                /* common header file */


#ifdef I2C_BITBANG

/*
 *  HINTs:
 *  - port and pins for bit-bang I2C
 *    I2C_PORT      port data register
 *    I2C_DDR       port data direction register
 *    I2C_PIN       port input pins register
 *    I2C_SDA       pin for SDA
 *    I2C_SCL       pin for SCL
 *  - For hardware I2C (TWI) the MCU specific pins are used:
 *    ATmega 328: SDA PC4 / SCL PC5 
 *    ATmega 644: SDA PC1 / SCL PC0
 *  - bus speed modes
 *    I2C_STANDARD_MODE  100kHz
 *    I2C_FAST_MODE      400kHz
 *  - Don't forget the pull up resistors for SDA and SCL!
 *    Usually 2-10kOhms for 5V.
 */

/*
 *  local constants
 */

I2C_Type        I2C;                     /* I2C */


/*
 *  bus speed modes:
 *  - standard mode      up to 100kbit/s
 *  - fast mode          up to 400kbit/s
 *  - fast mode plus     up to 1Mbit/s      
 *  - high-speed mode
 *    400pF load         up to 1.7Mbit/s
 *    100pF load         up to 3.4Mbit/s
 *
 *  address modes:
 *  - 7 bit + R/W   ->   address byte
 *    R/W (1/0)          bit 0
 *    bit 0 (A0)         bit 1
 *    bit 1 (A1)         bit 2
 *    bit 2 (A2)         bit 3
 *    bits 3-6           bits 4-7
 *  - 10 bit + R/W  ->   2 address bytes
 *    R/W (1/0)          MSB bit 0
 *    bits 0-7           LSB bits 0-7
 *    bits 8-9           MSB bits 1-2
 *    11110              MSB bits 3-7
 */


/* ************************************************************************
 *   functions for bit-bang I2C
 * ************************************************************************ */

/*
 *  set up SDA and SCL lines
 *
 *  returns:
 *  - I2C_ERROR on bus error
 *  - I2C_OK on success
 */

uint8_t I2C_Setup(void)
{
  uint8_t           Bits;               /* register bits */

  /* default is SDA and SCL pulled up by external resistors */
  /* set both pins to HiZ input mode */
  I2C_DDR &= ~((1 << I2C_SDA) | (1 << I2C_SCL));

  /* preset pins to 0 */
  I2C_PORT &= ~((1 << I2C_SDA) | (1 << I2C_SCL));

  /* check if SDA and SCL are pulled up externally */
  Bits = I2C_PIN;                  /* get state */
  Bits &= (1 << I2C_SDA) | (1 << I2C_SCL);        /* filter lines */
  if (Bits == ((1 << I2C_SDA) | (1 << I2C_SCL)))  /* lines are high */
    return I2C_OK;       /* signal success */

  /*
   *  current state:
   *  - SDA high
   *  - SCL high
   */

  return I2C_ERROR;
}


/*
 *  create start condition
 *  - Type
 *    I2C_START for start condition
 *    I2C_REPEATED_START for repeated start condition (not enabled yet)
 *
 *  returns:
 *  - I2C_ERROR on bus error
 *  - I2C_OK on success
 */

uint8_t I2C_Start(uint8_t Type)
{
  uint8_t           Bits;               /* register bits */

  if (Type == I2C_START)      /* start */
  {
    /*
     *  expected state:
     *  - SDA high
     *  - SCL high
     */

    /*
     *  TODO: clock hold
     *  - check if SCL is pulled down by slave
     *  - wait until slave releases SCL
     */

    Bits = I2C_DDR;                /* get mode of IO pins */

    /* pull down SDA (SDA low) */
    Bits |= (1 << I2C_SDA);        /* set output mode */
    I2C_DDR = Bits;                /* enable pull down */

    /* SCL will follow after t_HD;STA */
#ifdef I2C_FAST_MODE
    /* fast mode: min. 0.6탎 */
    wait1us();
#else
    /* standard mode: min. 4탎 */
    wait4us();
#endif

    /* pull down SCL (SCL low) */
    Bits |= (1 << I2C_SCL);        /* set output mode */
    I2C_DDR = Bits;                /* enable pull down */

    /*
     *  current state:
     *  - SDA low
     *  - SCL low
     */
  }
#if 0
  else                        /* repeated start */
  {
    /*
     *  expected state:
     *  - SDA undefined
     *  - SCL low
     */

    /*
     *  First part similar to stop condition, besides SDA is released
     *  as first action (instead of last).
     */

    /* release SDA (SDA high) */
    I2C_DDR &= ~(1 << I2C_SDA);    /* set to HiZ (disable pull-down) */

    /* SCL has to stay low for t_LOW */
#ifdef I2C_FAST_MODE
    /* fast mode: min. 1.3탎 */
    wait2us();
#else
    /* standard mode: min. 4.7탎 */
    wait5us();
#endif

    /* release SCL (SCL high) */
    I2C_DDR &= ~(1 << I2C_SCL);    /* set to HiZ (disable pull-down) */

  /* SDA has to stay high for t_SU;STA after SCL rises */
#ifdef I2C_FAST_MODE
    /* fast mode: min. 0.6탎 */
    wait1us();
#else
    /* standard mode: min. 4.7탎 */
    wait5us();
#endif

    /*
     *  current state:
     *  - SDA high
     *  - SCL high
     */

    /*
     *  Second part follows common start condition.
     *  Same code as above for I2C_START.
     */

    /* ... code from above (I2C_START) ... */

    /*
     *  current state:
     *  - SDA low
     *  - SCL low
     */
  }
#endif // 0

  /* current state:
   * - SDA low 
   * - SCL low
   */

  return I2C_OK;
}


/*
 *  write byte (master to slave)
 *  - send byte (address or data) to slave
 *    bit-bang 8 bits, MSB first, LSB last
 *  - get ACK/NACK from slave
 *  - byte to be sent is taken from global I2C.Byte
 *
 *  requires:
 *  - Type
 *    I2C_DATA for data byte
 *    I2C_ADDRESS for address byte
 *
 *  returns:
 *  - I2C_ERROR for bus error
 *  - I2C_ACK for ACK
 *  - I2C_NACK NACK
 */

uint8_t I2C_WriteByte(uint8_t Type)
{
  uint8_t           Ret = I2C_ERROR;    /* return value */
  uint8_t           Byte;               /* byte */
  uint8_t           n = 8;              /* counter */
  #ifdef I2C_RW
  uint8_t           ReadFlag = 0;       /* flag for read mode */
  #endif

  /*
   *  expected state:
   *  - SDA undefined
   *  - SCL low
   */

  /*
   *  send byte
   */

  Byte = I2C.Byte;            /* get byte */

#ifdef I2C_RW
  /* check for read mode */
  if (Type == I2C_ADDRESS)         /* address byte */
  {
    if (Byte & 0b00000001)         /* read mode */
      ReadFlag = 1;                /* set flag */
  }
#endif

  /* bit-bang 8 bits */
  while (n > 0)               /* 8 bits */
  {
    /*
     *  set SDA
     */

    /* SDA has to be set t_SU;DAT before SCL rises */
    /* fast mode: min. 100ns */
    /* standard mode: min. 250ns */

    /* get current MSB and set SDA */
    if (Byte & 0b10000000)    /* 1 */
      /* set SDA high by releasing it */
      I2C_DDR &= ~(1 << I2C_SDA);       /* set to HiZ (disable pull-down) */
    else                      /* 0 */
      /* set SDA low by pulling it down */
      I2C_DDR |= (1 << I2C_SDA);        /* set to output mode (enable pull-down) */

    /*
     *  run SCL cycle
     */

    /* SCL has to stay low for t_LOW */
#ifdef I2C_FAST_MODE
    /* fast mode: min. 1.3탎 */
    wait2us();
#else
    /* standard mode: min. 4.7탎 */
    wait5us();
#endif

    /* release SCL (SCL high) */
    I2C_DDR &= ~(1 << I2C_SCL);         /* set to HiZ (disable pull-down) */

    /* 
     *  TODO: clock stretching (inter-bit)
     *  - check if SCL is pulled down by slave
     *  - wait until slave releases SCL
     */

    /* SCL has to stay high for t_HIGH */
#ifdef I2C_FAST_MODE
    /* fast mode: min. 0.6탎 */
    wait1us();
#else
    /* standard mode: min. 4탎 */
    wait4us();
#endif

    /* pull down SCL (SCL low) */
    I2C_DDR |= (1 << I2C_SCL);          /* set to output mode (enable pull-down) */

    /* SDA has to stay valid for t_HD;DAT after SCL falls */
    /* fast mode: min. 300ns */
    /* standard mode: min. 300ns */
    wait1us();

    /* new SDA has to become valid in t_VD;DAT after SCL falls */
    /* fast mode: max. 0.9탎 */
    /* standard mode: max. 3.45탎 */

    Byte <<= 1;               /* shift bits one step left */
    n--;                      /* next bit */
  }

  /*
   *  current state:
   *  - SDA undefined
   *  - SCL low
   */

  /*
   *  get ACK/NACK
   */

  /*
   *  expected state:
   *  - SDA undefined
   *  - SCL low
   */

  /* release SDA (SDA high) for slave */
  I2C_DDR &= ~(1 << I2C_SDA);           /* set to HiZ (disable pull-down) */


  /*
   *  start SCL cycle for ACK/NACK
   */

  /* SCL has to stay low for t_LOW */
#ifdef I2C_FAST_MODE
  /* fast mode: min. 1.3탎 */
  wait2us();
#else
  /* standard mode: min. 4.7탎 */
  wait5us();
#endif

  /*
   *  Some sources say that the slave has to pull down SDA for an ACK already
   *  before the master releases SCL. So we could wait here for a low SDA or
   *  timeout?
   */

  /* release SCL (SCL high) */
  I2C_DDR &= ~(1 << I2C_SCL);           /* set to HiZ (disable pull-down) */

  /*
   *  we expect slave to pull down SDA (SDA low) to ACK
   *  - some slaves might need some time, like an EEPROM for writing
   *    or an ADC for the conversion
   *  - if slave doesn't pull SDA down within some time limit it's a NACK
   */

  /* set timeout */
  Byte = I2C.Timeout;         /* get timeout */
  Byte *= 2;                  /* convert into 5탎 steps */
  if (Byte == 0)              /* prevent zero timeout */
    Byte++;                   /* set to 1 (5탎) */

  /* wait for ACK (SDA low) or timeout */
  while (Byte > 0)            /* timeout loop */
  {
    Byte--;                   /* decrease timeout counter */

    n = I2C_PIN;              /* get current state */
    n &= (1 << I2C_SDA);      /* filter SDA pin */

    if (n == 0)               /* SDA low */
    {
      Ret = I2C_ACK;                 /* signal ACK */
      Byte = 0;                      /* end loop */
    }
    else                             /* SDA high */
    {
      if (Byte > 0)                  /* no timeout yet */
        wait5us();                   /* wait */
    }
  }

  /* timeout means NACK */
  if (Ret == I2C_ERROR)      /* got no ACK (still default) */
    Ret = I2C_NACK;          /* signal NACK */

  /* for an ACK the slave has to keep SDA low during the clock pulse */

  /* 
   *  TODO: clock stretching (inter-byte)
   *  - check if SCL is pulled down by slave
   *  - wait until slave releases SCL
   */  

  /* SCL has to stay high for t_HIGH */
#ifdef I2C_FAST_MODE
  /* fast mode: min. 0.6탎 */
  wait1us();
#else
  /* standard mode: min. 4탎 */
  wait4us();
#endif

  /* TODO: should we check if SDA is still low? */

  /* pull down SCL (SCL low) */
  I2C_DDR |= (1 << I2C_SCL);       /* set to output mode (enable pull-down) */

  /* SDA has to stay valid for t_HD;DAT after SCL falls */
  /* fast mode: min. 300ns */
  /* standard mode: min. 300ns */
  wait1us();

#ifdef I2C_RW
  if (ReadFlag == 0)          /* write mode */
  {
#endif

    /* slave has to release SDA (SDA high) */
    /* so we check this */
    n = I2C_PIN;                        /* get current state */
    n &= (1 << I2C_SDA);                /* filter SDA pin */
    if (n == 0)                         /* SDA low */
      /* slave hasn't released SDA */
      Ret = I2C_ERROR;                  /* signal error */

#ifdef I2C_RW
  }
#endif

  /*
   *  current state:
   *  - SDA high
   *  - SCL low
   */

  return Ret;
}


#ifdef I2C_RW

/*
 *  read byte (master from slave)
 *  - read byte (data) from slave
 *    bit-bang 8 bits, MSB first, LSB last
 *  - ACK/NACK to slave
 *  - byte to be read is stored in global I2C.Byte
 *
 *  requires:
 *  - Type
 *    I2C_ACK for sending acknowledge
 *    I2C_NACK for sending not-acknowledge
 *
 *  returns:
 *  - I2C_ERROR on bus error
 *  - I2C_OK on success
 */

uint8_t I2C_ReadByte(uint8_t Type)
{
  uint8_t           Byte = 0;           /* byte */
  uint8_t           n = 8;              /* counter */

  /*
   *  expected state:
   *  - SDA undefined
   *  - SCL low
   */

  /*
   *  read byte
   */

  /* release SDA for slave (SDA high) */
  I2C_DDR &= ~(1 << I2C_SDA);      /* set to HiZ (disable pull-down) */

  /* bit-bang 8 bits */
  while (n > 0)               /* 8 bits */
  {
    /*
     *  run SCL cycle
     */

    /* SCL has to stay low for t_LOW */
#ifdef I2C_FAST_MODE
    /* fast mode: min. 1.3탎 */
    wait2us();
#else
    /* standard mode: min. 4.7탎 */
    wait5us();
#endif

    /* release SCL (SCL high) */
    I2C_DDR &= ~(1 << I2C_SCL);    /* set to output mode (enable pull-down) */

    /* SCL has to stay high for t_HIGH */
#ifdef I2C_FAST_MODE
    /* fast mode: min. 0.6탎 */
    wait1us();
#else
    /* standard mode: min. 4탎 */
    wait4us();
#endif

    /* pull down SCL (SCL low) */
    I2C_DDR |= (1 << I2C_SCL);     /* set to output mode (enable pull-down) */

    /* SDA has to stay valid for t_HD;DAT after SCL falls */
    /* fast mode: min. 300ns */
    /* standard mode: min. 300ns */

    /* new SDA has to become valid in t_VD;DAT after SCL falls */
    /* fast mode: max. 0.9탎 */
    /* standard mode: max. 3.45탎 */

    /* read SDA */
    if (I2C_PIN & (1 << I2C_SDA))       /* SDA high */
      Byte |= 0b00000001;               /* set bit */

    n--;                      /* next bit */

    if (n > 0)
      Byte <<= 1;             /* shift 1 bit left */
  }

  I2C.Byte = Byte;            /* save byte */

  /*
   *  current state:
   *  - SDA undefined
   *  - SCL low
   */

  /*
   *  send ACK/NACK
   */

  /*
   *  expected state:
   *  - SDA undefined
   *  - SCL low
   */

  /* check if slave has released SDA */
  n = I2C_PIN;                     /* get current state */
  n &= (1 << I2C_SDA);             /* filter SDA pin */
  if (n == 0)                      /* SDA low */
    /* slave hasn't released SDA */
    return I2C_ERROR;

  /* SDA has to become valid in t_VD;ACK after read's last SCL fall */
  /* fast mode: max. 0.9탎 */
  /* standard mode: max. 3.45탎 */

  /* set SDA */
  if (Type == I2C_ACK)        /* ACK */
  {
    /* pull down SDA (SDA low) to signal ACK to slave */
    I2C_DDR |= (1 << I2C_SDA);     /* set to output mode (enable pull-down) */
  }

  /* else: NACK */
  /* we keep SDA high to signal NACK to slave */


  /*
   *  run SCL cycle for ACK/NACK
   */

  /* SCL has to stay low for t_LOW */
  #ifdef I2C_FAST_MODE
    /* fast mode: min. 1.3탎 */
    wait2us();
  #else
    /* standard mode: min. 4.7탎 */
    wait5us();
  #endif

  /* release SCL (SCL high) */
  I2C_DDR &= ~(1 << I2C_SCL);      /* set to HiZ (disable pull-down) */

  /* SCL has to stay high for t_HIGH */
  #ifdef I2C_FAST_MODE
    /* fast mode: min. 0.6탎 */
    wait1us();
  #else
    /* standard mode: min. 4탎 */
    wait4us();
  #endif

  /* pull down SCL (SCL low) */
  I2C_DDR |= (1 << I2C_SCL);       /* set to output mode (enable pull-down) */

  /* SDA has to stay valid for t_HD;DAT after SCL falls */
  /* fast mode: min. 300ns */
  /* standard mode: min. 300ns */
  wait1us();

  /*
   *  current state:
   *  - SDA undefined
   *  - SCL low
   */

  return I2C_OK;
}

#endif // I2C_RW



/*
 *  create stop condition
 */

void I2C_Stop(void)
{
  uint8_t           Bits;     /* register bits */

  /*
   *  expected state:
   *  - SDA undefined
   *  - SCL low
   */

  Bits = I2C_DDR;                  /* get IO pin mode */

  /* pull down SDA (SDA low) */
  Bits |= (1 << I2C_SDA);          /* set output mode */
  I2C_DDR = Bits;                  /* enable pull-down */

  /* SCL has to stay low for t_LOW */
  /* actually t_LOW - t_HD;DAT after ACK */
#ifdef I2C_FAST_MODE
  /* fast mode: min. 1.3탎 */
  wait2us();
#else
  /* standard mode: min. 4.7탎 */
  wait5us();
#endif

  /* release SCL (SCL high) */
  Bits &= ~(1 << I2C_SCL);         /* set to input mode (disable pull-down) */
  I2C_DDR = Bits;                  /* enable HiZ */

  /* SDA will follow after t_SU;STO */
#ifdef I2C_FAST_MODE
  /* fast mode: min. 0.6탎 */
  wait1us();
#else
  /* standard mode: min. 4탎 */
  wait4us();
#endif

  /* release SDA (SDA high) */
  Bits &= ~(1 << I2C_SDA);         /* set input mode (disable pull-down) */
  I2C_DDR = Bits;                  /* enable HiZ */

  /*
   *  bus free time between Stop and next Start (T_BUF)
   *  - standard mode: min. 4.7탎
   *  - fast mode: min. 1.3탎
   */

  /*
   *  current state:
   *  - SDA high
   *  - SCL high
   */
}

#endif // I2C_BITBANG
