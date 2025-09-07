/* ************************************************************************
 *
 *   alternative 24x24 component symbols #1
 *   vertically aligned, horizontal bit order flipped
 *   with bank/page-wise grouping
 *
 *   (c) 2015-2023 by Markus Reschke
 *   modified by indman@EEVblog
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   symbol data
 * ************************************************************************ */

#ifdef SYMBOLS_24X24_ALT1_VP_F


#define SYMBOLS_SELECTED

/* symbol size */
#define SYMBOL_SIZE_X       24     /* width:  24 dots */
#define SYMBOL_SIZE_Y       24     /* height: 24 dots */

/* symbol data format */
#define SYMBOL_BYTES_N      72     /* 72 bytes per symbol */
#define SYMBOL_BYTES_X       3     /* 3 bytes in x direction */
#define SYMBOL_BYTES_Y      24     /* 24 bytes in y direction */


extern const uint8_t SymbolData[];
extern const uint8_t PinTable[];


#endif // SYMBOLS_24X24_ALT1_VP_F
