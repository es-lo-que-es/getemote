#include "stdio.h"

#include "auth_handler.h"
#include "unistd.h"


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

   print_slist(get_auth_headers());

   release_request_handler();
   release_auth_handler();

   printf("app runs\n");
   return 0;
}
