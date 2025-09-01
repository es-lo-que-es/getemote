#include "auth_handler.h"

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


/*
static bool populate_client_headers(AuthHandler *self)
{
   struct curl_slist *h0 = append_file_to_slist(CLIENT_ID_PATH, "client_id", NULL);
   if ( h0 == NULL ) return false;

   struct curl_slist *h1h0 = append_file_to_slist(CLIENT_SECRET_PATH, "client_secret", h0);
   if ( h1h0 == NULL ) { curl_slist_free_all(h0); return false; }

   struct curl_slist *h2h1h0 = curl_slist_append(h1h0, GRANT_TYPE_STR);
   if ( h2h1h0 == NULL ) { curl_slist_free_all(h1h0); return false; }

   self->client_headers = h2h1h0;
   return true;
}
*/


bool init_auth_handler()
{
   AuthHandler *self = &s_auth_handler;
   memset(self, 0, sizeof(AuthHandler));
   
   if ( !read_file(CLIENT_ID_PATH, self->client_id, sizeof(self->client_id)) ) return false;
   if ( !read_file(CLIENT_SECRET_PATH, self->client_secret, sizeof(self->client_secret)) ) return false;

   const char *data = TextFormat("client_id=%s&client_secret=%s&%s", 
         self->client_id, self->client_secret, GRANT_TYPE_STR);

   const int len = strlen(data) >= sizeof(self->post_data) ? sizeof(self->post_data) - 1 : strlen(data);
   memcpy(self->post_data, data, strlen(data));

   self->auth_req = make_post_request(TOKEN_REQ_URL, self->post_data, NULL);
   if ( self->auth_req == NULL ) return false;
   
   AUTH_HANDLER_INITIALISED = true;
   return true;
}


enum AuthState wait_for_authorisation()
{
   assert(AUTH_HANDLER_INITIALISED);

   AuthHandler *self = &s_auth_handler;
   if ( self->done ) return AuthSuccess;
   if ( self->failed ) return AuthFailed;

   handle_requests();
   if ( self->auth_req->done ) {
   
      if ( self->auth_req->failed ) {
         fprintf(stderr, "auth request has failed: \n");
         fprintf(stderr, "reason: '%s'\n", get(&self->auth_req->resp, 0));
         release_request(self->auth_req);
         self->failed = true;
         return AuthFailed;
      } else {
         printf("auth success!\n");
         printf("resp: '%s'\n", get(&self->auth_req->resp, 0));
         // TODO: parse auth response to create auth headers
         self->done = true;
         return AuthSuccess;
      }
   }

   return AuthWait;
}
