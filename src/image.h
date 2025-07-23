#ifndef IMAGE_H
#define IMAGE_H

extern image image_import(const char *path);
extern void image_deport(image img);
extern color image_sample(image img, u32 x, u32 y);
extern void image_draw(image img, i16 x, i16 y);
extern void image_draw_frame(image img, i16 x, i16 y, u32 frame);

#endif