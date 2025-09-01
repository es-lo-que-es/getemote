#ifndef REQUEST_H_HEADER
#define REQUEST_H_HEADER

#include "curl/curl.h"
#include "stdbool.h"
#include "stdint.h"
#include "cc.h"


typedef struct Request {
   vec(uint8_t) resp;
   bool failed, done;
   CURL *ehandle;
} Request;


Request *_alloc_post_request(const char *url, const char *data, struct curl_slist *headers);
Request *_alloc_get_request(const char *url, struct curl_slist *headers);
void _free_request(Request *request);


#endif
