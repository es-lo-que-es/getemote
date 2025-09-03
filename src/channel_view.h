#ifndef CHANNEL_VIEW_HEADER_H
#define CHANNEL_VIEW_HEADER_H

#include "reqs/channel_lookup.h"
#include "emote_list.h"
#include "raylib.h"


typedef struct ChannelView {
   ChannelLookup lookup;
   EmoteList emotes;
} ChannelView;


void request_channel_view(ChannelView *chview, const char *username);
void draw_channel_view(ChannelView *chview, Rectangle r);

void release_channel_view(ChannelView *chview);
void init_channel_view(ChannelView *chview);

#endif
