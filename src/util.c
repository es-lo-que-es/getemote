#include "util.h"

#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "raylib.h"


#define RAD120 2.0944
void draw_loading_animation(Rectangle r, Color fg)
{
   const Vector2 o = { r.x + r.width * 0.5, r.y + r.height * 0.5 };
   const float now = GetTime() * 3;

   const float rad = r.height * 0.25;
   const float drad = rad * 0.10;

   const Vector2 a = { o.x + cosf(now) * rad, o.y + sinf(now) * rad };
   const Vector2 b = { o.x + cosf(now + RAD120) * rad, o.y + sinf(now + RAD120) * rad };
   const Vector2 c = { o.x + cosf(now - RAD120) * rad, o.y + sinf(now - RAD120) * rad };
   
   DrawCircleV(o, drad * 2.5, fg);
   DrawCircleV(a, drad, fg);
   DrawCircleV(b, drad, fg);
   DrawCircleV(c, drad, fg);
}


Rectangle scale_rec(Rectangle r, float scale)
{
   const float h = r.height * scale;
   const float w = r.width * scale;

   const Rectangle res = { 
      r.x + (r.width - w) * 0.5,
      r.y + (r.height - h) * 0.5,
      w, h
   };

   return res;
}


Rectangle pad_rec(Rectangle r, float value)
{
   const float h = r.height - value * 2;
   const float w = r.width - value * 2;

   const Rectangle res = { 
      r.x + value, r.y + value,
      w, h
   };

   return res;
}


void do_call(VCallback cb)
{
   if ( cb.call != NULL ) cb.call(cb.arg);
}


// WARNING: this is insecure and platform specific implementation
// basically u can redefine xclip to whatever command u need and this app will execute it
// but since i doubt anyone but me will ever use this for know it will do just fine
static void dump_to_clipboard(unsigned char *data, int size)
{
   FILE* xclip = popen("xclip -selection clipboard -t image/png", "w");
   if ( xclip == NULL ) return;

   fwrite(data, 1, size, xclip);
   pclose(xclip);
}


void copy_image(Image source, Rectangle r)
{
   Image img = ImageCopy(source);

   if ( !IsImageValid(img) ) return;
   ImageResize(&img, r.width, r.height);

   int data_size = 0;
   unsigned char *buffer = ExportImageToMemory(img, ".png", &data_size);

   if ( buffer != NULL ) {
      dump_to_clipboard(buffer, data_size);
      free(buffer);
   }

   UnloadImage(img);
}


bool valid_index(int idx, int len)
{
   return idx >= 0 && idx < len;
}


void draw_centered_text(const char *text, Rectangle r, int fsize, Color col)
{
   const float w = fmaxf(MeasureText(text, fsize), 1);

   const float y = r.y + (r.height - fsize) * 0.5;
   const float x = r.x + (r.width - w) * 0.5;

   DrawText(text, x, y, fsize, col);
}
