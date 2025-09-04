#ifndef CONFIG_HEADER_H
#define CONFIG_HEADER_H

#include "raylib.h"


typedef struct Config {

   int window_height;
   int window_width;
   float emote_padding;

   Color fg_alt, bg_alt;
   Color fg, bg;

} Config;


const char *local_path(const char *filename);
Rectangle app_rec();

extern const Config *gconfig;
void load_config();

#define EMOTES_PER_ROW 7

#endif
