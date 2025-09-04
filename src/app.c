#include "app.h"

#include "util.h"
#include "raylib.h"
#include "config.h"
#include "resources.h"
#include "reqs/auth_handler.h"


void release_app(App *self)
{
   release_interface(&self->ui);
   release_resources();
   CloseWindow();
}


static void init_window()
{
   InitWindow(gconfig->window_width, gconfig->window_height, "getemote");
   SetConfigFlags(FLAG_WINDOW_UNDECORATED);

   SetExitKey(KEY_NULL);
   SetTargetFPS(30);
}


static void search_enter_cb(App *self)
{
   const char *value = search_bar_value(&self->ui.search);
   request_channel_view(&self->ui.channel_view, value);
}


bool init_app(App *self)
{
   if ( !load_resources() ) return false;
   init_window();

   init_interface(&self->ui);
   // INFO: setting up ui callbacks
   self->ui.search.on_enter = (VCallback){.call = (void*)search_enter_cb, .arg = self};

   return true;
}


static void draw_auth_waiting_screen()
{
   const Rectangle r = { 0, 0, gconfig->window_width, gconfig->window_height };
   draw_loading_animation(r, gconfig->bg_alt);
}


static void draw_auth_failed_screen()
{
   const int fsize = gconfig->window_height * 0.06;
   const char *text = "._. AUTH FAILED ._.";

   const int w = MeasureText(text, fsize);
   const int y = (gconfig->window_height - fsize) * 0.5;
   const int x = (gconfig->window_width - w) * 0.5;
   DrawText(text, x, y, fsize, gconfig->fg);
}


static void draw_app(App *self)
{
   draw_interface(&self->ui);
}


void run_app(App *self)
{
   handle_requests();
   int auth = wait_for_authorisation();

   BeginDrawing();
      ClearBackground(gconfig->bg);
      if ( auth == AuthWait ) draw_auth_waiting_screen();
      else if ( auth == AuthFailed ) draw_auth_failed_screen();
      else draw_app(self);
   EndDrawing();
}
