#ifndef RECENT_VIEW_HEADER_H
#define RECENT_VIEW_HEADER_H

#include "emotes/emote_list.h"
#include "raylib.h"

typedef struct RecentView {

   EmoteList emotes;
   vec(char*) names;

   int selected;
   Rectangle r;

} RecentView;


void update_recent_view(RecentView *rview, const char *name);

void init_recent_view(RecentView *rview, Rectangle r);
void release_recent_view(RecentView *rview);

void draw_recent_view(RecentView *rview);


#endif
