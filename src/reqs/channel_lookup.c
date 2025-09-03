#include "channel_lookup.h"

#include "twitch_requests.h"
#include "string.h"


void reset_channel_lookup(ChannelLookup *self)
{
   release_request(self->emotes_req);
   release_request(self->id_req);

   self->state = NoLookup;

   self->emotes_req = NULL;
   self->id_req = NULL;
}


void lookup_channel_emotes(ChannelLookup *self, const char *username)
{
   reset_channel_lookup(self);
   self->state = LookupWait;

   self->id_req = request_broadcaster_id(username);

   if ( self->id_req == NULL ) {
      self->state = LookupFailed; 
   }
}


static void process_id_response(ChannelLookup *self)
{
   char *id = parse_broadcaster_id(self->id_req->resp);
   if ( id == NULL ) { self->state = LookupFailed; return; }

   self->emotes_req = request_emote_list(id); 
   free(id);

   if ( self->emotes_req == NULL ) {
      self->state = LookupFailed; 
   }
}


void handle_channel_lookup(ChannelLookup *self)
{
   if ( self->state != LookupWait ) return;
   if ( self->id_req == NULL ) return;

   // send emotes request
   if ( self->id_req->done && self->emotes_req == NULL ) {
      return process_id_response(self);
   }

   if ( self->emotes_req != NULL ) {
      if ( self->emotes_req->done ) self->state = LookupDone;
   }
}


void release_channel_lookup(ChannelLookup *self)
{
   release_request(self->emotes_req);
   release_request(self->id_req);
}


void init_channel_lookup(ChannelLookup *self)
{
   memset(self, 0, sizeof(ChannelLookup));
}


vec(EmoteInfo) consume_lookup_result(ChannelLookup *self)
{
   vec(EmoteInfo) result = { 0 };
   init(&result);
   
   if ( self->emotes_req != NULL ) {
      if ( !empty_response(self->emotes_req) ) {
         result = parse_emote_list(self->emotes_req->resp);
      }
   }

   reset_channel_lookup(self);

   return result;
}
