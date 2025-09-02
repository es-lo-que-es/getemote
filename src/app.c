#include "app.h"

#include "raylib.h"
#include "emote.h"
#include "config.h"
#include "resources.h"


Emote emote = { 0 };


void release_app(App *self)
{
   release_resources();
   CloseWindow();
}


static void init_window()
{
   InitWindow(gconfig->window_width, gconfig->window_height, "getemote");
   SetConfigFlags(FLAG_WINDOW_UNDECORATED);

   //SetExitKey(KEY_NULL);
   SetTargetFPS(30);
}


bool init_app(App *self)
{
   if ( !load_resources() ) return false;
   init_window();

   EmoteInfo info = { 0 };
   init_emote_info(&info, "https://static-cdn.jtvnw.net/emoticons/v2/303256306/static/light/3.0", "shroudChill");
   init_emote(&emote, info);

   return true;
}


void run_app(App *self)
{
   const Rectangle r = { 100, 100, 256, 256 };
   handle_requests();

   BeginDrawing();
      ClearBackground(RAYWHITE);
      draw_emote(&emote, r);
   EndDrawing();
}
