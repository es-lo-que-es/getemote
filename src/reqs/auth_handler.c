#include "auth_handler.h"

#include "jansson.h"
#include "assert.h"
#include "raylib.h"
#include "stdio.h"


#define TOKEN_REQ_URL "https://id.twitch.tv/oauth2/token"
#define GRANT_TYPE_STR "grant_type=client_credentials"
#define CLIENT_SECRET_PATH "tokens/CLIENT_SECRET"
#define CLIENT_ID_PATH "tokens/CLIENT_ID"


static bool AUTH_HANDLER_INITIALISED = false;
static AuthHandler s_auth_handler = { 0 };


struct curl_slist *get_auth_headers()
{
   assert(AUTH_HANDLER_INITIALISED);

   AuthHandler *self = &s_auth_handler;
   return self->auth_headers;
}


void release_auth_handler()
{
   AuthHandler *self = &s_auth_handler;
   curl_slist_free_all(self->auth_headers);
}


static bool read_file(const char *path, char *buffer, int max)
{
   FILE *file = fopen(path, "rb");

   if ( file == NULL ) {
      fprintf(stderr, "failed to open '%s'\n", path);
      return false;
   }

   // read file contents into a buffer
   int byte_count = fread(buffer, 1, max - 1, file);
   buffer[byte_count] = 0;
   fclose(file);

   // cut to the first newline if any
   for ( int i = byte_count - 1; i >= 0; --i ) {
      if ( buffer[i] == '\n' || buffer[i] == '\r' ) buffer[i] = 0;
   }
   
   return true;
}


bool init_auth_handler()
{
   AuthHandler *self = &s_auth_handler;
   memset(self, 0, sizeof(AuthHandler));
   
   if ( !read_file(CLIENT_ID_PATH, self->client_id, sizeof(self->client_id)) ) return false;
   if ( !read_file(CLIENT_SECRET_PATH, self->client_secret, sizeof(self->client_secret)) ) return false;

   const char *data = TextFormat("client_id=%s&client_secret=%s&%s", 
         self->client_id, self->client_secret, GRANT_TYPE_STR);

   self->auth_req = make_post_request(TOKEN_REQ_URL, data, NULL);
   if ( self->auth_req == NULL ) return false;
   
   AUTH_HANDLER_INITIALISED = true;
   return true;
}


static int process_empty_response(AuthHandler *self)
{
   fprintf(stderr, "auth request has received an empty response\n");
   self->failed = true;

   return AuthFailed;
}


static int process_auth_failure(AuthHandler *self)
{
   fprintf(stderr, "auth request has failed :( \n");
   fprintf(stderr, "reason: '%s'\n", get(&self->auth_req->resp, 0));
   self->failed = true;

   return AuthFailed;
}


static void parse_auth_response(AuthHandler *self)
{
   vec(uint8_t) data = self->auth_req->resp;

   json_error_t err = { 0 };
   json_t *doc = json_loads(get(&data, 0), 0, &err);
   if ( doc == NULL ) goto error;

   json_t *token = json_object_get(doc, "access_token");
   const char *str = json_string_value(token);
   if ( str == NULL ) goto error;

   const char *token_field = TextFormat("Authorization: Bearer %s", str);
   struct curl_slist *h0 = curl_slist_append(NULL, token_field);
   if ( h0 == NULL ) goto error;

   const char *id_field = TextFormat("Client-Id: %s", self->client_id);
   struct curl_slist *h1 = curl_slist_append(h0, id_field);
   if ( h1 == NULL ) { curl_slist_free_all(h0); goto error; }

   self->auth_headers = h1;
   json_decref(doc);

   return;

error:
   fprintf(stderr, "failed to parse auth response: '%s'\n", get(&data, 0));
   json_decref(doc);
}


static int process_auth_success(AuthHandler *self)
{
   parse_auth_response(self);
   return AuthSuccess;
}


enum AuthState wait_for_authorisation()
{
   int status = AuthWait;
   assert(AUTH_HANDLER_INITIALISED);

   AuthHandler *self = &s_auth_handler;
   if ( self->failed ) return AuthFailed;
   if ( self->done ) return AuthSuccess;

   handle_requests();
   if ( self->auth_req->done ) {
      if ( empty_response(self->auth_req) ) status = process_empty_response(self);
      else if ( self->auth_req->failed ) status = process_auth_failure(self);
      else status = process_auth_success(self);
      
      release_request(self->auth_req);
      self->done = true;
   }

   return status;
}
