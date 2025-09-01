#include "stdio.h"

#include "request_handler.h"
#include "unistd.h"


int main()
{
   if ( !init_request_handler() ) return -1;

   printf("active requests: %d\n", active_requests());
   handle_requests();
   Request *req = make_get_request("https://example.com", NULL);

   if ( req != NULL ) {

      printf("active requests: %d\n", active_requests());
      while ( !req->done ) {
         handle_requests();
         sleep(1);
      }

      if ( req->failed ) printf("request has failed!\n");
      if ( push(&req->resp, 0) ) printf("resp: '%s'\n", get(&req->resp, 0));
   }

   release_request(req);
   printf("active requests: %d\n", active_requests());

   handle_requests();
   release_request_handler();

   printf("app runs\n");
   return 0;
}
