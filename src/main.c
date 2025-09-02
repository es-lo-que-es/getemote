#include "stdio.h"

#include "twitch_requests.h"
#include "auth_handler.h"
#include "unistd.h"
#include "raylib.h"


static void print_slist(struct curl_slist *list)
{
   while ( list ) {
      printf("%s\n", list->data);
      list = list->next;
   }
}


int main()
{
   if ( !init_request_handler() ) return -1;
   
   if ( !init_auth_handler() ) {
      release_request_handler(); 
      return -1;
   }

   int status = AuthWait; 
   while ( status == AuthWait ) {
      status = wait_for_authorisation();
      sleep(1);
   }
   
   Request *req = request_broadcaster_id("twitch");

   while ( !req->done ) {
      handle_requests();
      sleep(1);
   }

   char *id = parse_broadcaster_id(req->resp);
   if ( id ) printf("twitch broadcaster id: %s\n", id);

   req = request_emote_list(id);
   while ( !req->done ) {
      handle_requests();
      sleep(1);
   }
   
   vec(EmoteInfo) emotes = parse_emote_list(req->resp);
   for ( int i = 0; i < size(&emotes); ++i ) {
      printf("name: %s; url: [%s]\n", get(&emotes, i)->name, get(&emotes, i)->url);
   }

   cleanup(&emotes);
   free(id);

   release_request_handler();
   release_auth_handler();

   return 0;
}
