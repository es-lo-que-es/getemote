#ifndef CONFIG_HEADER_H
#define CONFIG_HEADER_H

#include "raylib.h"


typedef struct Config {

   int window_height;
   int window_width;

   Color fg, bg;
   Color bg_alt;

} Config;


const char *local_path(const char *filename);

extern const Config *gconfig;
void load_config();

#endif
