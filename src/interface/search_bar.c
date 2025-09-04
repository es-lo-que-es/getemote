#include "search_bar.h"
#include "string.h"


void init_search_bar(SearchBar *self, Rectangle r)
{
   memset(self, 0, sizeof(SearchBar));
   init_input_line(&self->inp);
   self->r = r;
}


void draw_search_bar(SearchBar *self)
{
   draw_input_line(&self->inp, self->r);
   if ( IsKeyPressed(KEY_ENTER) ) do_call(self->on_enter);
   if ( self->inp.updated ) do_call(self->on_input);
}


const char *search_bar_value(SearchBar *self)
{
   return self->inp.buffer;
}
