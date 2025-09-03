#include "channel_view.h"

#include "config.h"
#include "string.h"

#include "util.h"
#include "math.h"


void release_channel_view(ChannelView *self)
{
   release_channel_lookup(&self->lookup);
   release_emote_list(&self->emotes);
}


void init_channel_view(ChannelView *self)
{
   memset(self, 0, sizeof(ChannelView));
   init_channel_lookup(&self->lookup);
   init_emote_list(&self->emotes);
}


void request_channel_view(ChannelView *self, const char *username)
{
   lookup_channel_emotes(&self->lookup, username);
   clear_emote_list(&self->emotes);
}


static void draw_channel_emotes(ChannelView *self, Rectangle sr)
{
   const int emotes_per_row = 7;
   sr = pad_rec(sr, (sr.width / emotes_per_row) * gconfig->emote_padding);

   const float fsize = fmaxf(sr.height * 0.05, 14);
   DrawText("channel emotes:", sr.x, sr.y, fsize, gconfig->fg);
   sr.y += fsize * 1.25;

   const float side = sr.width / emotes_per_row;
   const float required_width = side * emotes_per_row;
   Rectangle r = { sr.x, sr.y, side, side };

   int i = 0;
   const int len = emote_count(&self->emotes);

   while ( i < len ) {
      for ( int j = 0; j < emotes_per_row && i < len; ++j ) {
         draw_emote(get_emote_at(&self->emotes, i), r);
         r.x += side;
         ++i;
      }

      r.y += side;
      r.x = sr.x; 
   }
}


void draw_channel_view(ChannelView *self, Rectangle r)
{
   if ( self->lookup.state == LookupWait ) {
      draw_loading_animation(r, gconfig->bg_alt);

   } else if ( self->lookup.state == LookupDone ) {
      fill_emote_list_from_info(&self->emotes, consume_lookup_result(&self->lookup));

   } else if ( self->lookup.state == LookupFailed ) {
      DrawText("._. lookup has failed ._.", 20, 20, 24, gconfig->fg);

   } else {
      draw_channel_emotes(self, r);
   }

   handle_channel_lookup(&self->lookup);
}
