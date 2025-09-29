#ifndef URL_LOOKUP_HEADER_H
#define URL_LOOKUP_HEADER_H

void append_url_lookup(const char *name, const char *url);
const char *lookup_url(const char *name);

void release_url_lookup();
void load_url_lookup();

#endif
