#include "request_handler.h"
#include "stdio.h"
#include "assert.h"


static bool REQUEST_HANDLER_INITIALISED = false;
static RequestHandler s_request_handler = { 0 };


void release_request(Request *req)
{
   assert(REQUEST_HANDLER_INITIALISED);
   if ( req == NULL ) return;

   RequestHandler *self = &s_request_handler;
   curl_multi_remove_handle(self->mhandle, req->ehandle);
   erase(&self->requests, (uintptr_t)req->ehandle);
   _free_request(req);
}


void release_request_handler()
{
   assert(REQUEST_HANDLER_INITIALISED);
   RequestHandler *self = &s_request_handler;

   for( Request **el = first(&self->requests); el != end(&self->requests); el = next(&self->requests, el) ) {
      Request *req = *el;
      curl_multi_remove_handle(self->mhandle, req->ehandle);
      _free_request(req);
   }

   REQUEST_HANDLER_INITIALISED = false;
   curl_multi_cleanup(self->mhandle);
   cleanup(&self->requests);

   curl_global_cleanup();
}


bool init_request_handler()
{
   RequestHandler *self = &s_request_handler;
   memset(self, 0, sizeof(RequestHandler));

   curl_global_init(CURL_GLOBAL_ALL);
   self->mhandle = curl_multi_init();

   if ( self->mhandle == NULL ) {
      fprintf(stderr, "failed to init curl multi handle\n");
      curl_global_cleanup();
      return false;
   }

   REQUEST_HANDLER_INITIALISED = true;

   init(&self->requests);
   return true;
}


static void process_message(RequestHandler *self, struct CURLMsg *msg)
{
   CURL *ehandle = msg->easy_handle;
   curl_multi_remove_handle(self->mhandle, ehandle);

   Request *req = *get(&self->requests, (uintptr_t)ehandle);
   
   // this should never happen since all handles are asociated with their request objects
   if ( req == NULL ) return curl_easy_cleanup(ehandle);

   if ( msg->data.result != CURLE_OK ) {
      fprintf(stderr, "request failed with error: '%s'\n", curl_easy_strerror(msg->data.result));
      req->failed = true;
   }

   // INFO: make sure all responses are NULL terminated 
   if ( !push(&req->resp, 0) ) if ( size(&req->resp) > 0 ) *last(&req->resp) = 0; 
   // WARNING: empty response + bad alloc could cause issues ._.

   req->done = true;
}


void handle_requests()
{
   assert(REQUEST_HANDLER_INITIALISED);
   RequestHandler *self = &s_request_handler;

   int running_handles = 0;
   curl_multi_perform(self->mhandle, &running_handles);

   // this exists only cus i dont like do->while loops
   struct CURLMsg stub = { 0 }; 
   struct CURLMsg *msg = &stub;
   int qlen = 0;

   while ( msg != NULL ) {
      msg = curl_multi_info_read(self->mhandle, &qlen);

      if ( msg != NULL && msg->msg == CURLMSG_DONE ) {
         process_message(self, msg);
      }
   }
}


Request *make_get_request(const char *url, struct curl_slist *headers)
{
   assert(REQUEST_HANDLER_INITIALISED);
   RequestHandler *self = &s_request_handler;

   Request *req = _alloc_get_request(url, headers);
   if ( req == NULL ) return NULL;

   int err = curl_multi_add_handle(self->mhandle, req->ehandle);
   if ( err ) { _free_request(req); return NULL; }

   if ( !insert(&self->requests, (uintptr_t)req->ehandle, req) ) {
      curl_multi_remove_handle(self->mhandle, req->ehandle);
      _free_request(req);
      return NULL;
   }

   return req;
}


Request *make_post_request(const char *url, const char *data, struct curl_slist *headers)
{
   assert(REQUEST_HANDLER_INITIALISED);
   RequestHandler *self = &s_request_handler;

   Request *req = _alloc_post_request(url, data, headers);
   if ( req == NULL ) return NULL;

   int err = curl_multi_add_handle(self->mhandle, req->ehandle);
   if ( err ) { _free_request(req); return NULL; }

   if ( !insert(&self->requests, (uintptr_t)req->ehandle, req) ) {
      curl_multi_remove_handle(self->mhandle, req->ehandle);
      _free_request(req);
      return NULL;
   }

   return req;
}


int active_requests()
{
   assert(REQUEST_HANDLER_INITIALISED);

   RequestHandler *self = &s_request_handler;
   return size(&self->requests);
}


Request *make_locking_get_request(const char *url, struct curl_slist *headers)
{
   Request *req = _alloc_get_request(url, headers);
   if ( req == NULL ) return NULL;
   
   int err = curl_easy_perform(req->ehandle);
   if ( err ) req->failed = true;

   req->done = true;
   return req;
}
