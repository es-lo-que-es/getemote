#include "recent_view.h"

#include "single_view.h"
#include "config.h"
#include "stdlib.h"
#include "string.h"

#include "util.h"
#include "math.h"


static void clear_names(RecentView *self)
{
   for ( int i = 0; i < size(&self->names); ++i ) {
      free(*get(&self->names, i));
   }

   clear(&self->names);
}


void release_recent_view(RecentView *self)
{
   release_emote_list(&self->emotes);
   clear_names(self);
   cleanup(&self->names);
}


static void swap_names(RecentView *self, int a, int b)
{
   char **pa = get(&self->names, a);
   char **pb = get(&self->names, b);

   char *tmp = *pa;
   *pa = *pb;
   *pb = tmp;
}


static void load_file_names(RecentView *self)
{
   FilePathList list = LoadDirectoryFiles(local_path("cache"));

   for ( int i = 0; i < list.count; ++i ) {
      char *name = strdup(GetFileNameWithoutExt(list.paths[i]));
      if ( name != NULL ) if ( !push(&self->names, name) ) free(name);
   }

   // sort filenames
   for ( int i = 0; i < size(&self->names); ++i ) {
   
      int min = i;
      for ( int j = i + 1; j < size(&self->names); ++j ) {
         if ( strcmp(*get(&self->names, j), *get(&self->names, min)) < 0 ) min = j;
      }

      swap_names(self, i, min);
   }

   UnloadDirectoryFiles(list);
}


void init_recent_view(RecentView *self, Rectangle r)
{
   memset(self, 0, sizeof(RecentView));
   init_emote_list(&self->emotes);
   self->selected = -1;

   init(&self->names);
   load_file_names(self);

   self->r = r;
}


static bool starts_with(const char *a, const char *b)
{
   while ( *a != 0 && *b != 0 ) {
      if ( *a != *b ) return false;
      ++a; ++b;
   }

   return true;
}


static void add_cached_emote(RecentView *self, const char *name)
{
   EmoteInfo info = { 0 };
   if ( !init_emote_info(&info, "", name) ) return;

   Emote emote = { 0 }; 
   init_emote(&emote, info);

   if ( !add_emote_to_list(&self->emotes, emote) ) {
      release_emote(&emote);
   }
}


#define MAX_RECENT 3
#define RECENT_DELAY 0.5
void update_recent_view(RecentView *self, const char *name)
{
   clear_emote_list(&self->emotes);
   const int len = size(&self->names);
 
   // find 3 matches  
   for ( int i = 0; i < len; ++i ) {
      if ( starts_with(*get(&self->names, i), name) ) {

         add_cached_emote(self, *get(&self->names, i));

         for ( int j = 1; j < MAX_RECENT; ++j ) {
            if ( i + j >= len ) return;
            if ( starts_with(*get(&self->names, i + j), name) ) {
               add_cached_emote(self, *get(&self->names, i + j));
            }
         }
         
         return;
      }
   }

}


void draw_recent_view(RecentView *self)
{
   if ( IsKeyReleased(KEY_ESCAPE) ) self->selected = -1;

   if ( get_emote_at(&self->emotes, self->selected) ) {
      return draw_single_view(get_emote_at(&self->emotes, self->selected), self->r);
   }

   const int len = fminf(emote_count(&self->emotes), MAX_RECENT);
   if ( len == 0 ) return;

   const Rectangle r = pad_rec(self->r, (self->r.width / EMOTES_PER_ROW) * gconfig->emote_padding);
   const float fsize = fmaxf(r.height * 0.24, 14);
   DrawText("recent emotes:", r.x, r.y, fsize, gconfig->fg);

   const float side = r.height * 0.76;
   const float y = r.y + fsize;

   const float count = fmaxf(fminf(emote_count(&self->emotes), MAX_RECENT), 1);
   const float ew = r.width / count;
   float x = r.x + (ew - side) * 0.5;

   for ( int i = 0; i < len; ++i ) {
      const Rectangle er = { x, y, side, side };
      
      if ( IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ) {
         if ( CheckCollisionPointRec(GetMousePosition(), er) ) self->selected = i;
      }

      draw_emote(get_emote_at(&self->emotes, i), er);
      x += ew;
   }
}
