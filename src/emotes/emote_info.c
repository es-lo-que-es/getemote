#include "emote_info.h"

#include "string.h"
#include "stdlib.h"


bool init_emote_info(EmoteInfo *self, const char *url, const char *name)
{
   if ( url == NULL || name == NULL ) return false;
   memset(self, 0, sizeof(EmoteInfo));

   self->name = strdup(name);
   if ( name == NULL ) return false;

   self->url = strdup(url);
   if ( url == NULL ) {
      free(self->name);
      return false;
   }

   return true;
}


void release_emote_info(EmoteInfo *self)
{
   free(self->name);
   free(self->url);
}
