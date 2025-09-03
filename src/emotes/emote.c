#include "emote.h"

#include "raymath.h"
#include "config.h"
#include "util.h"


void release_emote(Emote *self)
{
   release_emote_info(&self->info);
   release_request(self->req);
   UnloadTexture(self->texture);
   UnloadImage(self->image);
}


static void set_failed_image(Emote *self)
{
   // TODO: pre-load image once and deep copy it
   self->image = LoadImage(local_path("assets/failed.png"));
   self->texture = LoadTextureFromImage(self->image);
   self->loaded = true;
}


static bool load_cached_image(Emote *self, const char *name)
{
   const char *path = TextFormat("%scache/%s.png", GetApplicationDirectory(), name);
   if ( !FileExists(path) ) return false;

   self->image = LoadImage(path);
   self->texture = LoadTextureFromImage(self->image);
   if ( self->texture.width == 0 ) return false;
   
   self->loaded = true;
   return true;
}


void init_emote(Emote *self, EmoteInfo info)
{
   memset(self, 0, sizeof(Emote));

   if ( !load_cached_image(self, info.name) ) {

      self->req = make_get_request(info.url, NULL);

      if ( self->req == NULL ) {
         set_failed_image(self);
      }
   }

   self->info = info;
}


static void draw_emote_loading_animation(Rectangle r)
{
   draw_loading_animation(r, gconfig->bg_alt);
}


static void cache_request_data(Emote *self)
{
   if ( !FileExists(local_path("cache")) ) MakeDirectory(local_path("cache"));

   FILE *file = fopen(TextFormat("%scache/%s.png", GetApplicationDirectory(), self->info.name), "wb");
   if ( file == NULL ) return;

   vec(uint8_t) data = self->req->resp;
   fwrite(get(&data, 0), 1, size(&data), file);
   fclose(file);
}


static void draw_loading_emote(Emote *self, Rectangle r)
{
   draw_emote_loading_animation(r);

   if ( !self->req->done ) return;
   if ( self->req->failed ) return set_failed_image(self);
   if ( empty_response(self->req) ) return set_failed_image(self);

   vec(uint8_t) data = self->req->resp;
   self->image = LoadImageFromMemory(".png", get(&data, 0), size(&data));
   self->texture = LoadTextureFromImage(self->image);

   if ( self->texture.width == 0 ) return set_failed_image(self);
   cache_request_data(self);

   self->loaded = true;
}


void draw_emote(Emote *self, Rectangle r)
{

   bool hover = CheckCollisionPointRec(GetMousePosition(), r);
   if ( hover ) DrawRectangleRec(r, gconfig->fg_alt);

   const Rectangle sr = scale_rec(r, 1.0f - gconfig->emote_padding);
   if ( !self->loaded ) return draw_loading_emote(self, sr);

   const Rectangle src = { 0, 0, self->texture.width, self->texture.height };
   DrawTexturePro(self->texture, src, sr, Vector2Zero(), 0, WHITE);
}
