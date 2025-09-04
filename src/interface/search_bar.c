#include "search_bar.h"
#include "string.h"


void init_search_bar(SearchBar *self)
{
   memset(self, 0, sizeof(SearchBar));
   init_input_line(&self->inp);
}


void draw_search_bar(SearchBar *self, Rectangle r)
{
   draw_input_line(&self->inp, r);
   if ( IsKeyPressed(KEY_ENTER) ) do_call(self->on_enter);
   if ( self->inp.updated ) do_call(self->on_input);
}
