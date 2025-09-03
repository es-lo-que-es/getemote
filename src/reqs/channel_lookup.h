#ifndef CHANNEL_LOOKUP_HEADER_H
#define CHANNEL_LOOKUP_HEADER_H

#include "request_handler.h"
#include "emote_info.h"


enum LookupState { NoLookup, LookupFailed, LookupDone, LookupWait };


typedef struct ChannelLookup {

   Request *emotes_req;
   Request *id_req;
   int state;

} ChannelLookup;


void lookup_channel_emotes(ChannelLookup *lookup, const char *chanel);
void handle_channel_lookup(ChannelLookup *lookup);

vec(EmoteInfo) consume_lookup_result(ChannelLookup *lookup);
void init_channel_lookup(ChannelLookup *lookup);

void release_channel_lookup(ChannelLookup *lookup);
void reset_channel_lookup(ChannelLookup *lookup);

#endif
