#ifndef TEXT_H
#define TEXT_H

#define newline() ({ state.cursor_y -= settings.font_line_spacing; state.cursor_x = -CANVAS_WIDTH / 2; })
#define cursor_reset() ({ state.cursor_x = -CANVAS_WIDTH / 2; state.cursor_y = CANVAS_HEIGHT / 2; })

extern void text_init();
extern void text_render();

#endif