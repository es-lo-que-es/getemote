#include "emote_list.h"

#include "raylib.h"
#include "config.h"


void clear_emote_list(EmoteList *self)
{
   for ( int i = 0; i < emote_count(self); ++i ) {
      Emote *emote = get_emote_at(self, i);
      release_emote(emote);
   }

   clear(&self->data);
}


void release_emote_list(EmoteList *self)
{
   clear_emote_list(self);
   cleanup(&self->data);
}


Emote *get_emote_at(EmoteList *self, int at)
{
   if ( at < 0 || at >= size(&self->data) ) return NULL;
   return get(&self->data, at);
}


int emote_count(EmoteList *self)
{
   return size(&self->data);
}


// consume vector of emote info to create list of emotes
void fill_emote_list_from_info(EmoteList *self, vec(EmoteInfo) infos)
{
   clear_emote_list(self);

   int len = size(&infos);
   for ( int i = 0; i < len; ++i ) {
      
      Emote emote = { 0 };
      init_emote(&emote, *get(&infos, i));

      if ( !push(&self->data, emote) ) {
         release_emote(&emote);
      }

   }

   cleanup(&infos);
}


void _draw_emote_list(EmoteList *self)
{
   const float side = 64;
   const float pad = side * 0.05;

   Rectangle r = { pad, pad, side, side };

   for ( int i = 0; i < emote_count(self); ++i ) {
      draw_emote(get_emote_at(self, i), r);

      r.x += side + pad;
      if ( r.x + side + pad > gconfig->window_width ) {
         r.x = pad; r.y += side + pad;
      }
   }
}


void init_emote_list(EmoteList *self)
{
   init(&self->data);
}


bool add_emote_to_list(EmoteList *self, Emote emote)
{
   return push(&self->data, emote);
}
