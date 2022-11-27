#ifndef MODEL_VIEWER_WINDOW_H
#define MODEL_VIEWER_WINDOW_H

#include <SDL_video.h>

extern bool quit;

void window_init();
void window_frame();
SDL_GLContext window_get_ctx();
int window_get_width();
int window_get_height();
float window_get_aspect_ratio();

#endif //MODEL_VIEWER_WINDOW_H
