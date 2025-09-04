#include "interface.h"
#include "config.h"


void release_interface(Interface *self)
{
   release_channel_view(&self->channel_view);
   release_recent_view(&self->recent_view);
}


#define SEARCH_HEIGHT 0.1f
static void setup_search_bar(Interface *self)
{
   const Rectangle r = app_rec();
   const float sh = r.height * SEARCH_HEIGHT;

   const Rectangle sr = scale_rec((Rectangle){ r.x, sh, r.width, sh }, 0.6);
   init_search_bar(&self->search, sr);
}


static void setup_recent_view(Interface *self)
{
   const Rectangle r = app_rec();
   const float sh = r.height * SEARCH_HEIGHT;

   const Rectangle vr = { r.x, sh * 2, r.width, sh * 2 };
   init_recent_view(&self->recent_view, vr);
}


static void setup_channel_view(Interface *self)
{
   const Rectangle r = app_rec();
   const float sh = r.height * SEARCH_HEIGHT;

   const Rectangle vr = { r.x, sh * 4, r.width, r.height - sh * 4 };
   init_channel_view(&self->channel_view, vr);
}


void init_interface(Interface *self)
{
   setup_channel_view(self);
   setup_recent_view(self);
   setup_search_bar(self);
}


void draw_interface(Interface *self)
{
   if ( self->channel_view.selected != -1 ) return draw_channel_view(&self->channel_view);
   if ( self->recent_view.selected != -1 ) return draw_recent_view(&self->recent_view);

   // WARNING: on imput callback will update recent_view freeing textueres that would be otherwise drawn
   // so handle the search bar first. this is a result of using implicit architecture and combining 
   // widget drawing logic with update logic
   draw_search_bar(&self->search);

   draw_channel_view(&self->channel_view);
   draw_recent_view(&self->recent_view);
}
