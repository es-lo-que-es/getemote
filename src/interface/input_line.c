#include "input_line.h"

#include "string.h"
#include "config.h"

#include "stdio.h"
#include "util.h"
#include "math.h"


void init_input_line(InputLine *self)
{
   memset(self, 0, sizeof(InputLine));
}


#define INPUT_DELETE_DELAY 0.12
static void handle_input_delete(InputLine *self)
{
   if ( IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_W) ) {
      memset(self->buffer, 0, sizeof(self->buffer));
      self->len = 0;
      return;
   }

   if ( !IsKeyDown(KEY_BACKSPACE) && !IsKeyDown(KEY_DELETE) ) return;
   if ( self->len <= 0 ) return;

   double now = GetTime();
   if ( now - self->last_delete < INPUT_DELETE_DELAY ) return;

   self->buffer[--self->len] = 0;
   self->last_delete = now;
}


static void handle_input_line(InputLine *self)
{
   handle_input_delete(self);
   int key = GetCharPressed();

   while ( key > 0 ) {
      if ( self->len >= MAX_INPUT_LINE ) return;
      if ( key <= 127 ) self->buffer[self->len++] = key;
      key = GetCharPressed();
   }
}


static void draw_bottom_line(Rectangle r)
{
   const float y = r.y + r.height;
   const float thick = fmaxf(r.height * 0.01, 2);
   DrawLineEx((Vector2){r.x, y}, (Vector2){r.x + r.width, y}, thick, gconfig->fg);
}


#define BLINKER_DELAY 0.5
static void draw_blinker(InputLine *self, Vector2 pos, float fsize)
{
   if ( self->blink ) {
      const float thick = fmaxf(fsize * 0.25, 2);
      const float offs = fmaxf(fsize * 0.3, 2);
      pos.x = pos.x + offs;

      DrawLineEx(pos, (Vector2){pos.x, pos.y + fsize}, thick, gconfig->fg);
   }

   double now = GetTime(); 
   if ( now - self->last_blink < BLINKER_DELAY  ) return;

   self->blink = !self->blink;
   self->last_blink = now;
}


void draw_input_line(InputLine *self, Rectangle r)
{
   handle_input_line(self);
   draw_bottom_line(r);
   r = scale_rec(r, 0.9);

   float fsize = r.height;
   const float w = fmaxf(MeasureText(self->buffer, fsize), 1);

   float scale = fminf(1.0f, r.width / w);
   fsize = fsize * scale;
   
   const float wu = MeasureText(self->buffer, fsize);
   const float y = r.y + (r.height - fsize);
   const float x = r.x + (r.width - wu) * 0.5;

   DrawText(self->buffer, x, y, fsize, gconfig->fg);
   draw_blinker(self, (Vector2){x + wu, y}, fsize);
}
