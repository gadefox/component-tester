#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

/*
 * misc
 */

#define COUNT(a)   (sizeof(a) / sizeof(a[0]))
#define delay(ms)  _delay_ms(ms)

/*
 * flags
 */

#define HI(w)  ((w) >> 8)
#define LO(w)  ((w) & 0xFF)
#define BV(b)  (1 << b)

#define SB(v, b)      v |= b
#define CB(v, b)      v &= ~(b)
#define TB(v, b)      v ^= f
#define ISB(v, b)     ((v) & (b))
#define SCB(c, v, b)  do { if (c) SB(v, b); else CB(v, b); } while(0)

#endif /* COMMON_H */
