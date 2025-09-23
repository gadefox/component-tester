#ifndef SEGM_H
#define SEGM_H

#define SEGMW   8
#define SEGMH   14
#define SEGMW2  (SEGMW + 2)
#define SEGMH2  (SEGMH + 2)

void segm_draw(int x, int y, uint16_t color, uint8_t value);
void segm_drawhex(uint8_t x, uint8_t y, uint16_t color, uint32_t value, uint8_t digits);
void segm_test(uint16_t color);

#endif /* SEGM_H */
