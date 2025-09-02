#ifndef APP_H_HEADER
#define APP_H_HEADER


typedef struct App {
   int state;
} App;


void release_app(App *app);
bool init_app(App *app);

void run_app(App *app);


#endif
