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

void screen_shot_rec(Rectangle r);
void do_call(VCallback cb);

#endif
