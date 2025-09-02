#include "twitch_requests.h"

#include "auth_handler.h"
#include "jansson.h"
#include "raylib.h"

#define EMOTES_REQ_URL "https://api.twitch.tv/helix/chat/emotes?broadcaster_id="
#define BROADCASTER_REQ_URL "https://api.twitch.tv/helix/users?login="


Request *request_broadcaster_id(const char *username)
{
   const char *url = TextFormat("%s%s", BROADCASTER_REQ_URL, username);
   return make_get_request(url, get_auth_headers());
}


Request *request_emote_list(const char *id)
{
   const char *url = TextFormat("%s%s", EMOTES_REQ_URL, id);
   return make_get_request(url, get_auth_headers());
}


char *parse_broadcaster_id(vec(uint8_t) resp)
{
   json_error_t err = { 0 };
   json_t *doc = json_loads(get(&resp, 0), 0, &err);

   if ( doc == NULL ) goto error;
   json_t *entry = json_array_get(json_object_get(doc, "data"), 0);
   json_t *id = json_object_get(entry, "id");
   const char *str = json_string_value(id); 

   if ( str == NULL ) goto error;
   char *result = strdup(str);
   json_decref(doc);

   return result;
   
error:
   fprintf(stderr, "failed to parse id response: '%s'\n", get(&resp, 0));
   json_decref(doc);

   return NULL;
}


vec(EmoteInfo) parse_emote_list(vec(uint8_t) resp)
{
   vec(EmoteInfo) result = { 0 }; init(&result);

   json_error_t err = { 0 };
   json_t *doc = json_loads(get(&resp, 0), 0, &err);
   if ( doc == NULL ) goto error;

   json_t *emotes = json_object_get(doc, "data");
   const size_t len = json_array_size(emotes);

   for ( int i = 0; i < len; ++i ) {
      
      json_t *entry = json_array_get(emotes, i);
      const char *name = json_string_value(json_object_get(entry, "name"));

      json_t *images = json_object_get(entry, "images");
      const char *url = json_string_value(json_object_get(images, "url_4x"));

      EmoteInfo info = { 0 };
      if ( !init_emote_info(&info, url, name) ) goto error;
      push(&result, info);
   }

   json_decref(doc);
   return result;

error:
   fprintf(stderr, "failed to parse emote list: '%s'\n", get(&resp, 0));
   json_decref(doc);

   return result;
}
