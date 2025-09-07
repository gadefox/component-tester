/* ************************************************************************
 *
 *   color definitions for display modules
 *
 *   (c) 2015-2023 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifndef COLORS_H
#define COLORS_H


/* ************************************************************************
 *   default colors
 * ************************************************************************ */

/* background color */
#define COLOR_BACKGROUND      COLOR_BLACK

/* standard pen color */
#define COLOR_PEN             COLOR_GREEN

/* titles */
#define COLOR_TITLE           COLOR_YELLOW

/* values */
#define COLOR_VALUE           COLOR_YELLOW

/* cursor */
#define COLOR_CURSOR          COLOR_YELLOW

/* infos (hello/bye) */
#define COLOR_INFO            COLOR_CYAN

/* warnings (not used yet) */
#define COLOR_WARN            COLOR_YELLOW

/* errors */
#define COLOR_ERROR           COLOR_RED

/* marker (selected item) */
#define COLOR_MARKER          COLOR_YELLOW

/* symbols */
#define COLOR_SYMBOL          COLOR_YELLOW

/* color codes for probe pins */
#define COLOR_PROBE_1         COLOR_YELLOW
#define COLOR_PROBE_2         COLOR_GREEN
#define COLOR_PROBE_3         COLOR_RED

/* battery status */
#define COLOR_BAT_OK          COLOR_GREEN
#define COLOR_BAT_WEAK        COLOR_YELLOW
#define COLOR_BAT_LOW         COLOR_RED


#endif // COLORS_H
