#include "interface.h"
#include "config.h"


void release_interface(Interface *self)
{
   release_channel_view(&self->channel_view);
}


#define SEARCH_HEIGHT 0.1f
static void setup_search_bar(Interface *self)
{
   const Rectangle r = app_rec();
   const float sh = r.height * SEARCH_HEIGHT;

   const Rectangle sr = scale_rec((Rectangle){ r.x, sh, r.width, sh }, 0.6);
   init_search_bar(&self->search, sr);
}


static void setup_channel_view(Interface *self)
{
   const Rectangle r = app_rec();
   const float sh = r.height * SEARCH_HEIGHT;

   const Rectangle vr = { r.x, sh * 2, r.width, r.height - sh * 2 };
   init_channel_view(&self->channel_view, vr);
}


void init_interface(Interface *self)
{
   setup_channel_view(self);
   setup_search_bar(self);
}


void draw_interface(Interface *self)
{
   draw_search_bar(&self->search);
   draw_channel_view(&self->channel_view);
}
