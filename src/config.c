#include "config.h"

#include "raylib.h"
#include "jansson.h"
#include "string.h"


#define DEF_WIDTH 600
#define DEF_HEIGHT 600


static Config config = { 0 };
const Config *gconfig = &config;


const char *local_path(const char *filename)
{
   return TextFormat("%s%s", GetApplicationDirectory(), filename);
}


static void save_default_config()
{
   FILE *file = fopen(local_path("config.json"), "wb");
   if ( file == NULL ) return;

   const char *DEFAULT_CONFIG_STR = TextFormat(
      "{ \"window_width\": %d, \"window_height\": %d }",
      DEF_WIDTH, DEF_HEIGHT
   );

   fwrite(DEFAULT_CONFIG_STR, 1, strlen(DEFAULT_CONFIG_STR), file);
   fclose(file);
}


static void set_default_config()
{
   config.window_height = DEF_HEIGHT;
   config.window_width = DEF_WIDTH;
}


void load_config()
{
   config.fg = (Color) { 0xD1, 0xD7, 0xE0, 0xFF };
   config.bg = (Color) { 0x15, 0x1B, 0x23, 0xFF };

   config.bg_alt = (Color) { 0x21, 0x28, 0x30, 0xFF };
   config.fg_alt = (Color) { 0x3f, 0x48, 0x5a, 0xFF };
   config.emote_padding = 0.3f;

   json_error_t err = { 0 };
   json_t *doc = json_load_file(local_path("config.json"), 0, &err);

   if ( doc == NULL ) {
      fprintf(stderr, "failed to parse config file: line: %d; text: %s\n", err.line, err.text);
      goto error;
   }

   config.window_width = json_integer_value(json_object_get(doc, "window_width"));
   config.window_height = json_integer_value(json_object_get(doc, "window_height"));

   if ( config.window_width < 100 || config.window_height < 100 ) goto error;
   json_decref(doc);

   return;

error:
   save_default_config();
   set_default_config();
   json_decref(doc);
}


Rectangle app_rec()
{
   return (Rectangle) { 0, 0, config.window_width, config.window_height };
}
