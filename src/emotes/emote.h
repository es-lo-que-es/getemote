#ifndef EMOTE_HEADER_H
#define EMOTE_HEADER_H

#include "raylib.h"
#include "emote_info.h"

#include "reqs/request_handler.h"


typedef struct Emote {

   Image image;
   Texture2D texture;
   Request *req;

   EmoteInfo info;
   bool loaded;

} Emote;


void copy_emote(Emote *emote, Rectangle r, int lvl);

void draw_emote_raw(Emote *emote, Rectangle r);
void draw_emote(Emote *emote, Rectangle r);

void init_emote(Emote *emote, EmoteInfo info);
void release_emote(Emote *emote);

#endif
