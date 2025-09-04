#include "channel_view.h"
#include "single_view.h"

#include "config.h"
#include "string.h"

#include "util.h"
#include "math.h"


void release_channel_view(ChannelView *self)
{
   release_channel_lookup(&self->lookup);
   release_emote_list(&self->emotes);
}


void init_channel_view(ChannelView *self, Rectangle r)
{
   memset(self, 0, sizeof(ChannelView));
   init_channel_lookup(&self->lookup);
   init_emote_list(&self->emotes);

   self->selected = -1;
   self->r = r;
}


void request_channel_view(ChannelView *self, const char *username)
{
   lookup_channel_emotes(&self->lookup, username);
   clear_emote_list(&self->emotes);
   self->yoffset = 0;
}


static void draw_channel_emotes(ChannelView *self)
{
   Rectangle sr = self->r;
   const int len = emote_count(&self->emotes);
   if ( len == 0 ) return;

   sr = pad_rec(sr, (sr.width / EMOTES_PER_ROW) * gconfig->emote_padding);
   const float fsize = fmaxf(sr.height * 0.05, 14);
   DrawText("channel emotes:", sr.x, sr.y, fsize, gconfig->fg);
   sr.y += fsize * 1.25;

   const float side = sr.width / EMOTES_PER_ROW;
   const float required_width = side * EMOTES_PER_ROW;
   Rectangle r = { sr.x, sr.y, side, side };

   int i = 0 + (self->yoffset * EMOTES_PER_ROW);

   while ( i < len ) {
      for ( int j = 0; j < EMOTES_PER_ROW && i < len; ++j ) {

         if ( IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ) {
            if ( CheckCollisionPointRec(GetMousePosition(), r) ) self->selected = i;
         }

         draw_emote(get_emote_at(&self->emotes, i), r);
         r.x += side;

         ++i;
      }

      r.y += side;
      r.x = sr.x; 
   }
}


static void handle_scroll(ChannelView *self)
{
   const int len = emote_count(&self->emotes);
   const int max = fmaxf((len / EMOTES_PER_ROW) - 1, 0);

   bool scroll_down = GetMouseWheelMove() < 0;
   bool scroll_up = GetMouseWheelMove() > 0;

   if ( IsKeyPressed(KEY_DOWN) || scroll_down ) {
      self->yoffset = fminf(self->yoffset + 1, max);
   }

   if ( IsKeyPressed(KEY_UP) || scroll_up ) {
      self->yoffset = fmaxf(self->yoffset - 1, 0);
   }
}


void draw_channel_view(ChannelView *self)
{
   if ( IsKeyReleased(KEY_ESCAPE) ) self->selected = -1;

   if ( get_emote_at(&self->emotes, self->selected) ) {
      return draw_single_view(get_emote_at(&self->emotes, self->selected));
   }

   if ( self->lookup.state == LookupWait ) {
      draw_loading_animation(self->r, gconfig->bg_alt);

   } else if ( self->lookup.state == LookupDone ) {
      fill_emote_list_from_info(&self->emotes, consume_lookup_result(&self->lookup));

   } else if ( self->lookup.state == LookupFailed ) {
      DrawText("._. lookup has failed ._.", 20, 20, 24, gconfig->fg);

   } else {
      draw_channel_emotes(self);
      handle_scroll(self);
   }

   handle_channel_lookup(&self->lookup);
}
