#include "emote.h"

#include "raymath.h"
#include "config.h"


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


void init_emote(Emote *self, EmoteInfo info)
{
   memset(self, 0, sizeof(Emote));

   self->req = make_get_request(info.url, NULL);
   if ( self->req == NULL ) set_failed_image(self);

   self->info = info;
}


static void draw_loading_animation(Rectangle r)
{
   // TODO: spinning circles or some shit
   // ...

   DrawRectangleRec(r, GREEN);
}


static void draw_loading_emote(Emote *self, Rectangle r)
{
   draw_loading_animation(r);

   if ( !self->req->done ) return;
   if ( self->req->failed ) return set_failed_image(self);
   if ( empty_response(self->req) ) return set_failed_image(self);

   vec(uint8_t) data = self->req->resp;
   self->image = LoadImageFromMemory(".png", get(&data, 0), size(&data));
   self->texture = LoadTextureFromImage(self->image);

   if ( self->texture.width == 0 ) return set_failed_image(self);
   self->loaded = true;
}


void draw_emote(Emote *self, Rectangle r)
{
   if ( !self->loaded ) return draw_loading_emote(self, r);
   const Rectangle src = { 0, 0, self->texture.width, self->texture.height };
   DrawTexturePro(self->texture, src, r, Vector2Zero(), 0, WHITE);
}
