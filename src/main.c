#include "app.h"
#include "raylib.h"


int main()
{
   App app = { 0 };
   if ( !init_app(&app) ) return -1;

   while ( !WindowShouldClose() ) run_app(&app);
   release_app(&app);

   return 0;
}
