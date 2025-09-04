#include "single_view.h"

#include "raylib.h"
#include "config.h"
#include "app.h"


static Rectangle get_centered_rec_at(const float y, const float w, const float s)
{
   const float x = (w - s) * 0.5;
   const Rectangle r = { x, y, s, s };
   return r;
}


static bool handle_rec_input(Rectangle r)
{
   const Vector2 pos = GetMousePosition();
   if ( !CheckCollisionPointRec(pos, r) ) return false;

   const float thick = r.width * 0.1;
   DrawRectangleLinesEx(scale_rec(r, 1.25), thick, gconfig->fg);

   bool released = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
   return released;
}


void draw_single_view(Emote *emote, Rectangle r)
{
   const float p = r.height * 0.1;

   const Rectangle r0 = get_centered_rec_at(r.y + p, r.width, 24);
   const Rectangle r1 = get_centered_rec_at(r0.y + r0.height + p, r.width, 48);
   const Rectangle r2 = get_centered_rec_at(r1.y + r1.height + p, r.width, 72);

   if ( handle_rec_input(r0) ) copy_emote(emote, r0);
   if ( handle_rec_input(r1) ) copy_emote(emote, r1);
   if ( handle_rec_input(r2) ) copy_emote(emote, r2);

   draw_emote_raw(emote, r0);
   draw_emote_raw(emote, r1);
   draw_emote_raw(emote, r2);
}
