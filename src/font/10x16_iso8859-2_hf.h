/* ************************************************************************
 *
 *   monospaced 10x16 font based on ISO 8859-2
 *   horizontally aligned, horizontal bit order flipped
 *
 *   (c) 2016-2023 by Markus Reschke
 *   Central European characters added by Bohu
 *   Inverted digits 1-3 by Feliciano
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_10X16_ISO8859_2_HF


/* source management */
#define FONT_SET                   /* font set included */

/* font size */
#define FONT_SIZE_X         10     /* width:  10 dots */
#define FONT_SIZE_Y         16     /* height: 16 dots */

/* font data format */
#define FONT_BYTES_N        32     /* 32 bytes per character */
#define FONT_BYTES_X         2     /* 2 bytes in x direction */
#define FONT_BYTES_Y        16     /* 16 bytes in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_10X16_ISO8859_2_HF
