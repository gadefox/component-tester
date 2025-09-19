/* ************************************************************************
 *
 *   thin monospaced 8x8 font based on Windows-1251
 *   vertically aligned, vertical bit order flipped
 *
 *   ASCII characters (c) 2015-2023 by Markus Reschke
 *   cyrillic characters added by Andrey@EEVblog
 *   Inverted digits 1-3 by Feliciano
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_8X8T_WIN1251_VF


/* source management */
#define FONT_SET                   /* font set included */

/* font size */
#define FONT_SIZE_X          8     /* width:  8 dots */
#define FONT_SIZE_Y          8     /* heigth: 8 dots */

/* font data format */
#define FONT_BYTES_N         8     /* 8 bytes per character */
#define FONT_BYTES_X         8     /* 8 bytes in x direction */
#define FONT_BYTES_Y         1     /* 1 byte in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_8X8T_WIN1251_VF
