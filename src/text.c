#include "ctagss.h"
#include "text.h"
#include "raster.h"
#include "image.h"

void text_init()
{
     cursor_reset();

     images.font = image_import("../res/images/font.bmp");
     images.font.h_frames = 94;
     images.font.v_frames = 1;
     images.font_shadow = image_import("../res/images/font_shadow.bmp");
     images.font_shadow.h_frames = 94;
     images.font_shadow.v_frames = 1;

     settings.font_char_spacing = 7;
     settings.font_word_spacing = 4;
     settings.font_line_spacing = 10;

     settings.font_shadow = 2;
}

static u8 text_char_lookup(char c)
{
     u8 index = 0;
     switch (c)
     {
     case 'A':
          break;
     case 'a':
          index = 1;
          break;
     case 'B':
          index = 2;
          break;
     case 'b':
          index = 3;
          break;
     case 'C':
          index = 4;
          break;
     case 'c':
          index = 5;
          break;
     case 'D':
          index = 6;
          break;
     case 'd':
          index = 7;
          break;
     case 'E':
          index = 8;
          break;
     case 'e':
          index = 9;
          break;
     case 'F':
          index = 10;
          break;
     case 'f':
          index = 11;
          break;
     case 'G':
          index = 12;
          break;
     case 'g':
          index = 13;
          break;
     case 'H':
          index = 14;
          break;
     case 'h':
          index = 15;
          break;
     case 'I':
          index = 16;
          break;
     case 'i':
          index = 17;
          break;
     case 'J':
          index = 18;
          break;
     case 'j':
          index = 19;
          break;
     case 'K':
          index = 20;
          break;
     case 'k':
          index = 21;
          break;
     case 'L':
          index = 22;
          break;
     case 'l':
          index = 23;
          break;
     case 'M':
          index = 24;
          break;
     case 'm':
          index = 25;
          break;
     case 'N':
          index = 26;
          break;
     case 'n':
          index = 27;
          break;
     case 'O':
          index = 28;
          break;
     case 'o':
          index = 29;
          break;
     case 'P':
          index = 30;
          break;
     case 'p':
          index = 31;
          break;
     case 'Q':
          index = 32;
          break;
     case 'q':
          index = 33;
          break;
     case 'R':
          index = 34;
          break;
     case 'r':
          index = 35;
          break;
     case 'S':
          index = 36;
          break;
     case 's':
          index = 37;
          break;
     case 'T':
          index = 38;
          break;
     case 't':
          index = 39;
          break;
     case 'U':
          index = 40;
          break;
     case 'u':
          index = 41;
          break;
     case 'V':
          index = 42;
          break;
     case 'v':
          index = 43;
          break;
     case 'W':
          index = 44;
          break;
     case 'w':
          index = 45;
          break;
     case 'X':
          index = 46;
          break;
     case 'x':
          index = 47;
          break;
     case 'Y':
          index = 48;
          break;
     case 'y':
          index = 49;
          break;
     case 'Z':
          index = 50;
          break;
     case 'z':
          index = 51;
          break;
     case ' ':
          index = 52;
          break;
     case '`':
          index = 53;
          break;
     case '~':
          index = 54;
          break;
     case '1':
          index = 55;
          break;
     case '!':
          index = 56;
          break;
     case '2':
          index = 57;
          break;
     case '@':
          index = 58;
          break;
     case '3':
          index = 59;
          break;
     case '#':
          index = 60;
          break;
     case '4':
          index = 61;
          break;
     case '$':
          index = 62;
          break;
     case '5':
          index = 63;
          break;
     case '%':
          index = 64;
          break;
     case '6':
          index = 65;
          break;
     case '^':
          index = 66;
          break;
     case '7':
          index = 67;
          break;
     case '&':
          index = 68;
          break;
     case '8':
          index = 69;
          break;
     case '*':
          index = 70;
          break;
     case '9':
          index = 71;
          break;
     case '(':
          index = 72;
          break;
     case '0':
          index = 73;
          break;
     case ')':
          index = 74;
          break;
     case '-':
          index = 75;
          break;
     case '_':
          index = 76;
          break;
     case '=':
          index = 77;
          break;
     case '+':
          index = 78;
          break;
     case '[':
          index = 79;
          break;
     case '{':
          index = 80;
          break;
     case ']':
          index = 81;
          break;
     case '}':
          index = 82;
          break;
     case '|':
          index = 84;
          break;
     case ';':
          index = 85;
          break;
     case ':':
          index = 86;
          break;
     case ',':
          index = 89;
          break;
     case '<':
          index = 90;
          break;
     case '.':
          index = 91;
          break;
     case '>':
          index = 92;
          break;
     case '/':
          index = 93;
          break;
     case '?':
          index = 94;
          break;
     }

     return index;
}

char text_digit_lookup(u8 num)
{
     char digit = '0';
     switch (num)
     {
     case 0:
          break;
     case 1:
          digit = '1';
          break;
     case 2:
          digit = '2';
          break;
     case 3:
          digit = '3';
          break;
     case 4:
          digit = '4';
          break;
     case 5:
          digit = '5';
          break;
     case 6:
          digit = '6';
          break;
     case 7:
          digit = '7';
          break;
     case 8:
          digit = '8';
          break;
     case 9:
          digit = '9';
          break;
     }

     return digit;
}

static void text_line(const char *phrase)
{
     while (*phrase)
     {
          u8 index = text_char_lookup(*phrase);
          (settings.font_shadow > 0) ? image_draw_frame(images.font_shadow, state.cursor_x + settings.font_shadow, state.cursor_y - settings.font_shadow, index) : (settings.font_shadow = settings.font_shadow);
          image_draw_frame(images.font, state.cursor_x, state.cursor_y, index);

          if (index != 52) 
          {
               state.cursor_x += settings.font_char_spacing;
          }else
          {
               state.cursor_x += settings.font_word_spacing;
          }

          phrase++;
     }
}

static void text_label(const char *phrase, i32 val)
{
     while (*phrase)
     {
          u8 index = text_char_lookup(*phrase);
          (settings.font_shadow > 0) ? image_draw_frame(images.font_shadow, state.cursor_x + settings.font_shadow, state.cursor_y - settings.font_shadow, index) : (settings.font_shadow = settings.font_shadow);
          image_draw_frame(images.font, state.cursor_x, state.cursor_y, index);

          if (index != 52)
          {
               state.cursor_x += settings.font_char_spacing;
          }
          else
          {
               state.cursor_x += settings.font_word_spacing;
          }

          phrase++;
     }

     char counter[8] = "00000000";

     u8 one, ten, hund, thou, ten_thou, hund_thou, mil, ten_mil;

     one = abs(val) % 10;
     ten = abs(val / 10) % 10;
     hund = abs(val / 100) % 10;
     thou = abs(val / 1000) % 10;
     ten_thou = abs(val / 10000) % 10;
     hund_thou = abs(val / 100000) % 10;
     mil = abs(val / 1000000) % 10;
     ten_mil = abs(val / 10000000) % 10;

     counter[0] = text_digit_lookup(ten_mil);
     counter[1] = text_digit_lookup(mil);
     counter[2] = text_digit_lookup(hund_thou);
     counter[3] = text_digit_lookup(ten_thou);
     counter[4] = text_digit_lookup(thou);
     counter[5] = text_digit_lookup(hund);
     counter[6] = text_digit_lookup(ten);
     counter[7] = text_digit_lookup(one);

     bool leading = true;

     for (u8 i = 0; i < 8; i++)
     {
          if (counter[i] != '0' && leading == true)
          {
               leading = false;
          }

          u8 index = text_char_lookup(counter[i]);
          if (leading == false)
          {
               (settings.font_shadow > 0) ? image_draw_frame(images.font_shadow, state.cursor_x + settings.font_shadow, state.cursor_y - settings.font_shadow, index) : (settings.font_shadow = settings.font_shadow);
               image_draw_frame(images.font, state.cursor_x, state.cursor_y, index);
               state.cursor_x += settings.font_char_spacing;
          }
     }
}

void text_render()
{
     text_line("Izaac Boho: Software Rasterizer");
     newline();
     text_line("(H TO HIDE/SHOW STATS, T TO LOCK/UNLOCK MOUSE)");
     if (settings.show_stats)
     {
        newline();
        text_label("Resolution: ",CANVAS_WIDTH);
        text_label("x",CANVAS_HEIGHT);
        newline();
        text_line("Bit Depth:");
        newline();
        text_label("  R:", settings.color_range_red);
        text_label(", G:", settings.color_range_green);
        text_label(", B:", settings.color_range_blue);
        newline();
        text_label("FPS:", stats.fps);
        newline();
        text_label("Total faces:", stats.total_faces);
        newline();
        text_label("Rendered faces:", stats.rendered_faces);
        newline();
        text_label("Point lights:", stats.point_lights);
        newline();
        text_label("Dir lights:", stats.dir_lights);
    }
    cursor_reset();
}
