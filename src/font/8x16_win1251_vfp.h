/* ************************************************************************
 *
 *   monospaced 8x16 font based on Windows-1251
 *   vertically aligned, vertical bit order flipped, bank-wise grouping
 *
 *   provided by indman@EEVblog and Gennady_13@VRTP.RU
 *   font (c) by NickNi@VRTP.RU
 *   cyrillic characters added by Andrey@EEVblog
 *   inverted digits 1-3 and x by indman
 *   battery symbols by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_8X16_WIN1251_VFP


/* source management */
#define FONT_SET                   /* font set included */

/* font size */
#define FONT_SIZE_X          8     /* width:   8 dots */
#define FONT_SIZE_Y         16     /* heigth: 16 dots */

/* font data format */
#define FONT_BYTES_N        16     /* 16 bytes per character */
#define FONT_BYTES_X         8     /*  8 bytes in x direction */
#define FONT_BYTES_Y         2     /*  2 bytes in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_8X16_WIN1251_VFP
