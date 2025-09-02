#ifndef EMOTE_INFO_HEADER_H
#define EMOTE_INFO_HEADER_H


typedef struct EmoteInfo {
   char *name, *url;
} EmoteInfo;

bool init_emote_info(EmoteInfo *eminfo, const char *url, const char *name);
void release_emote_info(EmoteInfo *eminfo);


#endif
