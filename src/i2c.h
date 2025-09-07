/* ************************************************************************
 *
 *   I2C functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#if defined(I2C_BITBANG) || defined(I2C_HARDWARE)

#ifndef I2C_H
#define I2C_H


/* I2C: either bit-bang or hardware */
#if defined(I2C_BITBANG) && defined(I2C_HARDWARE)
#error <<< I2C: select either bitbang or hardware I2C! >>>
#endif


#define I2C_ERROR             0         /* bus error */
#define I2C_OK                1         /* operation done */
#define I2C_START             1         /* start condition */
#define I2C_REPEATED_START    2         /* repeated start condition */
#define I2C_DATA              1         /* data byte */
#define I2C_ADDRESS           2         /* address byte */
#define I2C_ACK               1         /* acknowledge */
#define I2C_NACK              2         /* not-acknowledge */


typedef struct
{
  uint8_t           Byte;          /* address/data byte */
  uint8_t           Timeout;       /* ACK timeout in 10µs */
} I2C_Type;


extern uint8_t I2C_Setup(void);
extern uint8_t I2C_Start(uint8_t Type);
extern uint8_t I2C_WriteByte(uint8_t Type);
extern void I2C_Stop(void);

#ifdef I2C_RW
extern uint8_t I2C_ReadByte(uint8_t Type);
#endif // I2C_RW


extern I2C_Type      I2C;                /* I2C */

/* demodulated/raw IR code */
extern uint8_t       IR_Code[];          /* raw data */


#endif // I2C_H

#endif // I2C_BITBANG || I2C_HARDWARE
