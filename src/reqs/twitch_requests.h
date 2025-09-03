#ifndef TWITCH_REQUESTS_HEADER_H
#define TWITCH_REQUESTS_HEADER_H

#include "cc.h"
#include "request_handler.h"
#include "emotes/emote_info.h"

Request *request_broadcaster_id(const char *username);
Request *request_emote_list(const char *id);

vec(EmoteInfo) parse_emote_list(vec(uint8_t) data);
char *parse_broadcaster_id(vec(uint8_t) data);

#endif
