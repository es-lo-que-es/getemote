#ifndef AUTH_HANDLER_HEADER_H
#define AUTH_HANDLER_HEADER_H

#include "request_handler.h"

enum AuthState { AuthWait, AuthFailed, AuthSuccess };
#define AUTH_BUFFER_SIZE 256

typedef struct AuthHandler {

   char client_id[AUTH_BUFFER_SIZE];
   char client_secret[AUTH_BUFFER_SIZE];
   char post_data[AUTH_BUFFER_SIZE * 4];

   struct curl_slist *auth_headers;
   Request *auth_req;

   bool done, failed;

} AuthHandler;


enum AuthState wait_for_authorisation();
struct curl_slist *get_auth_headers();
bool init_auth_handler();

void release_auth_handler();


#endif
