#include "ctagss.h"
#include "image.h"
#include "raster.h"

image image_import(const char *path)
{
     image img;

     SDL_Surface *loaded = SDL_LoadBMP(path);
     img.source = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ABGR8888, 0);
     SDL_FreeSurface(loaded);

     img.pixels = img.source->pixels;
     img.width = img.source->w;
     img.height = img.source->h;

     img.h_frames = 1;
     img.v_frames = 1;

     return img;
}

void image_deport(image img)
{
     SDL_FreeSurface(img.source);
     img.source = NULL;
     img.pixels = NULL;
     img.width = 0;
     img.height = 0;
}

color image_sample(image img, u32 x, u32 y)
{
     u8 r, g, b, a;
     u32 pixel = img.pixels[y * img.source->pitch / 4 + x];
     SDL_GetRGBA(pixel, img.source->format, &r, &g, &b, &a);
     if (a == 255)
     {
          color col = {(f32)r / 255, (f32)g / 255, (f32)b / 255};
          return col;
     }else{
          color col = {-1, 0, 0};
          return col;
     }
}

void image_draw(image img, i16 x, i16 y)
{
     for (u16 sy = 0; sy < img.height; sy++)
     {
          for (u16 sx = 0; sx < img.width; sx++)
          {
               color col = image_sample(img, sx, sy);
               if (col.r != -1)
               {
                    raster_ppx_z(x + sx, y - sy, 5, col);
               }
          }
     }
}

void image_draw_frame(image img, i16 x, i16 y, u32 frame)
{
     u32 frame_width = img.width / img.h_frames;
     u32 frame_height = img.height / img.v_frames;

     for (u32 sy = 0; sy < frame_height; sy++)
     {
          for (u32 sx = 0; sx < frame_width; sx++)
          {
               color col = image_sample(img, sx + ((frame % img.h_frames) * frame_width), sy + ((frame / img.h_frames) * frame_height));
               if (col.r != -1)
               {
                    raster_ppx(x + sx, y - sy, col);
               }
          }
     }
}