#include "app.h"

#include "raylib.h"
#include "config.h"
#include "resources.h"


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

   return true;
}


void run_app(App *self)
{
   BeginDrawing();
      ClearBackground(RAYWHITE);
   EndDrawing();
}
