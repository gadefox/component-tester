/* ************************************************************************
 *
 *   monospaced 6x8 font based on ISO 8859-2
 *   horizontally aligned, horizontal bit order flipped
 *
 *   (c) 2015-2023 by Markus Reschke
 *   Central European characters added by Bohu
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/*
 *  include header files
 */

/* local includes */
#include "../common.h"           /* common header file */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_6X8_ISO8859_2_HF


/* source management */
#define FONT_SET                   /* font set included */

/* font size */
#define FONT_SIZE_X          6     /* width:  8 dots */
#define FONT_SIZE_Y          8     /* height: 8 dots */

/* font data format */
#define FONT_BYTES_N         8     /* 8 bytes per character */
#define FONT_BYTES_X         1     /* 1 byte in x direction */
#define FONT_BYTES_Y         8     /* 8 bytes in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_6X8_ISO8859_2_HF
