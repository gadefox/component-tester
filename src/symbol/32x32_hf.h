/* ************************************************************************
 *
 *   32x32 component symbols 
 *   horizontally aligned, horizontal bit order flipped
 *
 *   (c) 2015-2023 by Markus Reschke
 *   Question mark, Zener diode quartz crystal and OneWire device
 *   by indman@EEVblog
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   symbol data
 * ************************************************************************ */

#ifdef SYMBOLS_32X32_HF


#define SYMBOLS_SELECTED

/* symbol size */
#define SYMBOL_SIZE_X       32     /* width:  32 dots */
#define SYMBOL_SIZE_Y       32     /* height: 32 dots */

/* symbol data format */
#define SYMBOL_BYTES_N     128     /* 128 bytes per symbol */
#define SYMBOL_BYTES_X       4     /* 4 bytes in x direction */
#define SYMBOL_BYTES_Y      32     /* 32 bytes in y direction */


extern const uint8_t SymbolData[];
extern const uint8_t PinTable[];


#endif // SYMBOLS_32X32_HF
