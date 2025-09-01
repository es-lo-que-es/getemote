#include "request.h"
#include "string.h"


static size_t curl_write_cb(void *data, size_t esize, size_t n, void *arg)
{
   size_t bytecount = esize * n;
   vec(uint8_t) *resp = (vec(uint8_t)*)arg;

   uint8_t *pos = push_n(resp, data, bytecount);
   if ( pos == NULL ) return 0;

   return bytecount;
}


static bool init_get_request(Request *self, const char *url, struct curl_slist *headers)
{
   memset(self, 0, sizeof(Request));

   self->ehandle = curl_easy_init();
   if ( self->ehandle == NULL ) return false;

   curl_easy_setopt(self->ehandle, CURLOPT_WRITEFUNCTION, curl_write_cb);
   curl_easy_setopt(self->ehandle, CURLOPT_WRITEDATA, (void *)&self->resp);
   curl_easy_setopt(self->ehandle, CURLOPT_HTTPHEADER, headers);
   curl_easy_setopt(self->ehandle, CURLOPT_URL, url);
   init(&self->resp);

   return true;
}


Request *_alloc_get_request(const char *url, struct curl_slist *headers)
{
   Request *req = malloc(sizeof(Request));
   if ( req == NULL ) return NULL;

   if ( !init_get_request(req, url, headers) ) {
      free(req); return NULL;
   }

   return req;
}


// INFO: i could DRY it but i dont feel like i should
static bool init_post_request(Request *self, const char *url, const char *data, struct curl_slist *headers)
{
   memset(self, 0, sizeof(Request));

   self->ehandle = curl_easy_init();
   if ( self->ehandle == NULL ) return false;

   curl_easy_setopt(self->ehandle, CURLOPT_WRITEFUNCTION, curl_write_cb);
   curl_easy_setopt(self->ehandle, CURLOPT_WRITEDATA, (void *)&self->resp);
   curl_easy_setopt(self->ehandle, CURLOPT_HTTPHEADER, headers);
   curl_easy_setopt(self->ehandle, CURLOPT_POSTFIELDS, data);
   curl_easy_setopt(self->ehandle, CURLOPT_URL, url);
   init(&self->resp);

   return true;
}


Request *_alloc_post_request(const char *url, const char *data, struct curl_slist *headers)
{
   Request *req = malloc(sizeof(Request));
   if ( req == NULL ) return NULL;

   if ( !init_post_request(req, url, data, headers) ) {
      free(req); return NULL;
   }

   return req;
}


void _free_request(Request *self)
{
   curl_easy_cleanup(self->ehandle);
   cleanup(&self->resp);
   free(self);
}
