#ifndef APP_H_HEADER
#define APP_H_HEADER

#include "reqs/request_handler.h"
#include "channel_view.h"


typedef struct App {
   ChannelView channel_view;
} App;


void release_app(App *app);
bool init_app(App *app);

void run_app(App *app);


#endif
