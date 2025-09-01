#ifndef REQUEST_HANDLER_HEADER_H
#define REQUEST_HANDLER_HEADER_H

#include "request.h"

typedef struct RequestHandler {
   omap( uintptr_t, Request* ) requests;
   CURLM *mhandle;
} RequestHandler;


Request *make_get_request(const char *url, struct curl_slist *headers);
void release_request(Request *request);

void release_request_handler();
bool init_request_handler();
void handle_requests();

int active_requests();


#endif
