#include "channel_view.h"

#include "config.h"
#include "string.h"

#include "util.h"


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


void draw_channel_view(ChannelView *self, Rectangle r)
{
   if ( self->lookup.state == LookupWait ) {
      draw_loading_animation(r, gconfig->bg_alt);
   } else if ( self->lookup.state == LookupDone ) {
      fill_emote_list_from_info(&self->emotes, consume_lookup_result(&self->lookup));
   } else if ( self->lookup.state == LookupFailed ) {
      DrawText("._. lookup has failed ._.", 20, 20, 24, gconfig->fg);
   } else {
      _draw_emote_list(&self->emotes);
   }

   handle_channel_lookup(&self->lookup);
}
