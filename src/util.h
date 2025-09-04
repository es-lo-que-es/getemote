#ifndef UTIL_HEADER_H
#define UTIL_HEADER_H

#include "raylib.h"

void draw_loading_animation(Rectangle r, Color fg);
Rectangle scale_rec(Rectangle r, float scale);
Rectangle pad_rec(Rectangle r, float value);

typedef struct VCallback {
   void (*call)(void*);
   void *arg;
} VCallback;


// INFO: copies image to the clipboard
void copy_image(Image image, Rectangle r);

bool valid_index(int idx, int len);
void do_call(VCallback cb);

#endif
