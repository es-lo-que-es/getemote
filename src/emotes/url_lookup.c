#include "url_lookup.h"
#include "jansson.h"
#include "config.h"

static json_t *doc;


void append_url_lookup(const char *name, const char *url)
{
   json_object_set_new(doc, name, json_string(url));
}


const char *lookup_url(const char *name)
{
   return json_string_value(json_object_get(doc, name));
}


void release_url_lookup()
{
   const char *path = local_path("lookup.json");
   json_dump_file(doc, path, 0);
   json_decref(doc);
}


void load_url_lookup()
{
   json_error_t err;
   const char *path = local_path("lookup.json");
   doc = json_load_file(path, 0, &err);

   if ( doc == NULL ) {
      fprintf(stderr, "failed to parse '%s'. line:%d. msg: %s\n", path, err.line, err.text);
      doc = json_object();
   }
}
