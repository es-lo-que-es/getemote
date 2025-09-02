#include "app.h"
#include "raylib.h"


int main()
{
   /*
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
   */
   
   App app = { 0 };
   if ( !init_app(&app) ) return -1;

   while ( !WindowShouldClose() ) run_app(&app);
   release_app(&app);

   return 0;
}
