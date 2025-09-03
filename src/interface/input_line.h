#ifndef INPUT_LINE_HEADER_H
#define INPUT_LINE_HEADER_H

#include "raylib.h"
#define MAX_INPUT_LINE 32

typedef struct InputLine {
   
   char buffer[MAX_INPUT_LINE + 1];
   double last_delete;

   double last_blink;
   bool blink;
   int len;

} InputLine;

void draw_input_line(InputLine *input, Rectangle r);
void init_input_line(InputLine *input);

#endif
