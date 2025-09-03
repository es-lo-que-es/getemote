#include "util.h"
#include "math.h"


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
