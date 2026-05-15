/* ************************************************************************
 *
 *   thin monospaced 8x12 font based on ISO 8859-2
 *   horizontally aligned, horizontal bit order flipped
 *
 *   provided by indman@EEVblog and Gennady_13@VRTP.RU
 *   font (c) by NickNi@VRTP.RU
 *   Central European characters added by Bohu
 *   converted to "hf" by b0hoon
 *   Inverted digits 1-3 and x by Markus Reschke
 *   Battery symbols by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/* ************************************************************************
 *   font data
 * ************************************************************************ */

#ifdef FONT_8X12T_ISO8859_2_HF


/* source management */
#define FONT_SET                   /* font set included */

/* font size */
#define FONT_SIZE_X          8     /* width:   8 dots */
#define FONT_SIZE_Y         12     /* heigth: 12 dots */

/* font data format */
#define FONT_BYTES_N        12     /* 16 bytes per character */
#define FONT_BYTES_X         1     /*  8 bytes in x direction */
#define FONT_BYTES_Y        12     /*  2 bytes in y direction */


extern const uint8_t FontData[];
extern const uint8_t FontTable[];


#endif // FONT_8X12T_ISO8859_2_HF
