#ifndef SEARCH_BAR_HEADER_H
#define SEARCH_BAR_HEADER_H

#include "input_line.h"
#include "util.h"


typedef struct SearchBar {

   VCallback on_enter;
   VCallback on_input;

   InputLine inp;
   Rectangle r;

} SearchBar;


void init_search_bar(SearchBar *bar, Rectangle r);

const char *search_bar_value(SearchBar *bar);
void draw_search_bar(SearchBar *bar);

#endif
