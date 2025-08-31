#include "curl/curl.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "cc.h"


static const char *REQUEST_URL = "https://www.example.com";


static size_t curl_write_cb(void *data, size_t esize, size_t n, void *arg)
{
   size_t bytecount = esize * n;
   vec(uint8_t) *resp = (vec(uint8_t)*)arg;

   uint8_t *pos = push_n(resp, data, bytecount);
   if ( pos == NULL ) return 0;

   return bytecount;
}


int main()
{
   curl_global_init(CURL_GLOBAL_ALL);
   CURLM *mhandle = curl_multi_init();
   CURL *ehandle = curl_easy_init();

   vec(uint8_t) data = NULL;
   init(&data);

   if ( mhandle == NULL || ehandle == NULL ) {
      fprintf(stderr, "failed to init curl handle's\n");
      goto exit;
   }

   curl_easy_setopt(ehandle, CURLOPT_URL, REQUEST_URL);
   curl_easy_setopt(ehandle, CURLOPT_WRITEFUNCTION, curl_write_cb);
   curl_easy_setopt(ehandle, CURLOPT_WRITEDATA, (void *)&data);

   curl_multi_add_handle(mhandle, ehandle);
   int running_handles = 1;

   while ( running_handles > 0 ) {

      CURLMcode mc = curl_multi_perform(mhandle, &running_handles);

      if ( mc ) {
         fprintf(stderr, "curl_multi_prform has failed: '%s'\n", curl_multi_strerror(mc));
         break;
      }
    }

   if ( push(&data, 0) ) printf("response data: '%s'", get(&data, 0));

   curl_multi_remove_handle(mhandle, ehandle);
   curl_multi_cleanup(mhandle);
   curl_easy_cleanup(ehandle);
   cleanup(&data);

exit:
   curl_global_cleanup();
   return 0;
}
