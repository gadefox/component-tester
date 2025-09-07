/* ************************************************************************
 *
 *   monospaced 8x16 font based on Windows-1251
 *   horizontally aligned, bit order horizontally flipped
 *
 *   provided by indman@EEVblog and Gennady_13@VRTP.RU
 *   font (c) by NickNi@VRTP.RU
 *   inverted x by indman
 *   battery symbols and optimization by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_8X16_WIN1251_HF


/* source management */
#define FONT_SET                   /* font set included */

/* font size */
#define FONT_SIZE_X          8     /* width:   8 dots */
#define FONT_SIZE_Y         16     /* height: 16 dots */

/* font data format */
#define FONT_BYTES_N        16     /* 16 bytes per character */
#define FONT_BYTES_X         1     /*  1 bytes in x direction */
#define FONT_BYTES_Y        16     /* 16 bytes in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_8X16_WIN1251_HF
