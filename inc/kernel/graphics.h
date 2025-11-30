#ifndef __fulmonyx_16_graphics_h__
#define __fulmonyx_16_graphics_h__ 1

#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_BROWN         6
#define COLOR_LIGHT_GRAY    7
#define COLOR_DARK_GRAY     8
#define COLOR_LIGHT_BLUE    9
#define COLOR_LIGHT_GREEN   10
#define COLOR_LIGHT_CYAN    11
#define COLOR_LIGHT_RED     12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

void put_pixel(int x, int y, unsigned char color);
void print_char(char c);
void print_string(const char* str);
void set_video_mode(int mode);
void graphics_demo(void);
void draw_line(int x0, int y0, int x1, int y1, unsigned char color);
void draw_rectangle(int x, int y, int width, int height, unsigned char color);
void draw_circle(int x_center, int y_center, int radius, unsigned char color);

void put_pixel(int x, int y, unsigned char color) {
    if (x >= 320 || y >= 200) return;
    
    asm volatile (
        "int $0x10"
        :
        : "a"((0x0C << 8) | color),
          "c"(x),
          "d"(y)
        :
    );
}

/* 设置视频模式 - 精简修复 */
void set_video_mode(int mode) {
    asm volatile (
        "int $0x10"
        :
        : "a"((0x00 << 8) | (mode & 0xFF))
        :
    );
}

/* 文本模式下的字符打印 - 精简 */
void print_char(char c) {
    asm volatile (
        "int $0x10"
        :
        : "a"((0x0E << 8) | c)
        :
    );
}

/* 文本模式下的字符串打印 */
void print_string(const char* str) {
    while (*str) {
        print_char(*str++);
    }
}
void draw_line(int x0, int y0, int x1, int y1, unsigned char color) {//不依赖标准库
    int dx = x1 - x0;
    int dy = y1 - y0;
    int sx = (dx >= 0) ? 1 : -1;
    int sy = (dy >= 0) ? 1 : -1;
    dx = (dx >= 0) ? dx : -dx;
    dy = (dy >= 0) ? dy : -dy;

    if (dx > dy) {
        int err = dx / 2;
        for (; x0 != x1; x0 += sx) {
            put_pixel(x0, y0, color);
            err -= dy;
            if (err < 0) {
                y0 += sy;
                err += dx;
            }
        }
    } else {
        int err = dy / 2;
        for (; y0 != y1; y0 += sy) {
            put_pixel(x0, y0, color);
            err -= dx;
            if (err < 0) {
                x0 += sx;
                err += dy;
            }
        }
    }
    put_pixel(x1, y1, color);
}
void draw_rectangle(int x, int y, int width, int height, unsigned char color) {
    for (int i = 0; i < width; i++) {
        put_pixel(x + i, y, color);
        put_pixel(x + i, y + height - 1, color);
    }
    for (int j = 0; j < height; j++) {
        put_pixel(x, y + j, color);
        put_pixel(x + width - 1, y + j, color);
    }
}
void draw_circle(int x_center, int y_center, int radius, unsigned char color) {
    int x = radius;
    int y = 0;
    int p = 1 - radius;

    while (x > y) {
        y++;
        if (p <= 0) {
            p = p + 2*y + 1;
        } else {
            x--;
            p = p + 2*y - 2*x + 1;
        }
        if (x < y) break;

        put_pixel(x_center + x, y_center + y, color);
        put_pixel(x_center - x, y_center + y, color);
        put_pixel(x_center + x, y_center - y, color);
        put_pixel(x_center - x, y_center - y, color);
        put_pixel(x_center + y, y_center + x, color);
        put_pixel(x_center - y, y_center + x, color);
        put_pixel(x_center + y, y_center - x, color);
        put_pixel(x_center - y, y_center - x, color);
    }
}
#endif