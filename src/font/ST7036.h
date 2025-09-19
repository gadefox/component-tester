/* ************************************************************************
 *
 *   monospaced 5x8 font based on ISO 8859-1
 *   - uses internal font of ST7036
 *
 *   (c) 2019-2021 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_ST7036


/* source management */
#define FONT_SET                   /* font set included */

#ifdef FONT_EXTRA
#error <<< Font does not support extra characters! >>>
#endif

/* font size */
#define FONT_SIZE_X          5     /* width:  5 dots */
#define FONT_SIZE_Y          8     /* height: 8 dots */

/* font data format */
#define FONT_BYTES_N         8     /* 8 bytes per character */
#define FONT_BYTES_X         8     /* 8 bytes in x direction */
#define FONT_BYTES_Y         1     /* 1 byte in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_ST7036
