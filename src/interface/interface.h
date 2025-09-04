#ifndef INTERFACE_HEADER_H
#define INTERFACE_HEADER_H

#include "channel_view.h"
#include "recent_view.h"
#include "search_bar.h"


typedef struct Interface {
   ChannelView channel_view;
   RecentView recent_view;
   SearchBar search;
} Interface;


void release_interface(Interface *ifc);
void init_interface(Interface *ifc);

void draw_interface(Interface *ifc);

#endif
