/* ************************************************************************
 *
 *   monospaced 6x8 font based on ISO 8859-1
 *   vertically aligned, vertical bit order flipped
 *
 *   (c) 2016-2023 by Markus Reschke
 *   Inverted digits 1-3 by Feliciano
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_6X8_VF


/* source management */
#define FONT_SET                   /* font set included */

/* font size */
#define FONT_SIZE_X          6     /* width:  6 dots */
#define FONT_SIZE_Y          8     /* height: 8 dots */

/* font data format */
#define FONT_BYTES_N         6     /* 6 bytes per character */
#define FONT_BYTES_X         6     /* 8 bytes in x direction */
#define FONT_BYTES_Y         1     /* 1 byte in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_6X8_VF
