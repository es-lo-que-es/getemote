#include "stdio.h"

#include "request_handler.h"
#include "unistd.h"


int main()
{
   if ( !init_request_handler() ) return -1;

   printf("active requests: %d\n", active_requests());
   handle_requests();

   Request *req0 = make_get_request("https://example.com", NULL);
   Request *req1 = make_post_request("https://post.requestcatcher.com/test", "name=daniel&project=curl", NULL);

   if ( req0 != NULL && req1 != NULL ) {

      printf("active requests: %d\n", active_requests());
      while ( !req0->done || !req1->done ) {
         handle_requests();
         sleep(1);
      }

      if ( req0->failed ) printf("request0 has failed!\n");
      if ( push(&req0->resp, 0) ) printf("resp: '%s'\n", get(&req0->resp, 0));

      if ( req1->failed ) printf("request1 has failed!\n");
      if ( push(&req1->resp, 0) ) printf("resp: '%s'\n", get(&req1->resp, 0));
   }

   release_request(req0);
   printf("active requests: %d\n", active_requests());

   handle_requests();
   release_request_handler();

   printf("app runs\n");
   return 0;
}
