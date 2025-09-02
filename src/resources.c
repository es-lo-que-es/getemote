#include "resources.h"

#include "reqs/auth_handler.h"
#include "config.h"


void release_resources()
{
   release_request_handler();
   release_auth_handler();
}


bool load_resources()
{
   load_config();
   if ( !init_request_handler() ) return false;

   if ( !init_auth_handler() ) {
      release_request_handler();
      return false;
   }

   return true;
}
