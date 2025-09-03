#ifndef EMOTE_LIST_HEADER_H
#define EMOTE_LIST_HEADER_H

#include "emote.h"

typedef struct EmoteList {
   vec(Emote) data;
} EmoteList;

// consume vector of emote info to create list of emotes
void fill_emote_list_from_info(EmoteList *emlist, vec(EmoteInfo) infos);
void release_emote_list(EmoteList *emlist);
void init_emote_list(EmoteList *emlist);

Emote *get_emote_at(EmoteList *emlist, int at);
int emote_count(EmoteList *emlist);

void clear_emote_list(EmoteList *emlist);
void _draw_emote_list(EmoteList *emlist);

#endif
