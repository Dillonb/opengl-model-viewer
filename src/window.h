#ifndef MODEL_VIEWER_WINDOW_H
#define MODEL_VIEWER_WINDOW_H

#include <SDL_video.h>

extern bool quit;

void window_init();
void window_frame();
SDL_GLContext window_get_ctx();

#endif //MODEL_VIEWER_WINDOW_H
