/* ************************************************************************
 *
 *   monospaced 16x26 font based on ISO 8859-1
 *   horizontally aligned, horizontal bit order flipped
 *
 *   (c) 2015-2023 by Markus Reschke
 *   Inverted digits 1-3 and x by indman
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_16X26_HF


/* source management */
#define FONT_SET                   /* font set included */

/* font size */
#define FONT_SIZE_X         16     /* width:  16 dots */
#define FONT_SIZE_Y         26     /* height: 26 dots */

/* font data format */
#define FONT_BYTES_N        52     /* 52 bytes per character */
#define FONT_BYTES_X         2     /* 2 bytes in x direction */
#define FONT_BYTES_Y        26     /* 26 bytes in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_16X26_HF
