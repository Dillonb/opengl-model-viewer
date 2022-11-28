#include "window.h"
#include <iostream>
#include <SDL.h>
#define GLAD_GL_IMPLEMENTATION
#include <gl.h>

namespace {
    SDL_Window* win;
    SDL_GLContext ctx;

    int window_w = 640;
    int window_h = 480;
}

bool quit;

void window_on_resize(int new_w, int new_h) {
    window_w = new_w;
    window_h = new_h;

    glViewport(0, 0, window_w, window_h);

    printf("Resizing OpenGL viewport to (0, 0) (%d, %d)\n", window_w, window_h);
}

void window_init() {
    SDL_Init(SDL_INIT_VIDEO);
    win = SDL_CreateWindow("Model Viewer",
                           SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED,
                           window_w,
                           window_h,
                           SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    ctx = SDL_GL_CreateContext(win);
    SDL_GL_SetSwapInterval(1);

    gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
    std::cout << "Initialized SDL and loaded GL through GLAD" << std::endl;
    window_on_resize(window_w, window_h);
}

SDL_GLContext window_get_ctx() {
    return ctx;
}

void window_frame() {
    SDL_GL_SwapWindow(win);
    int new_w, new_h;
    SDL_GetWindowSize(win, &new_w, &new_h);
    if (new_w != window_w || new_h != window_h) {
        window_on_resize(new_w, new_h);
    }


    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
        }
    }
}

int window_get_width() {
    return window_w;
}

int window_get_height() {
    return window_h;
}

float window_get_aspect_ratio() {
    return (float)window_w/(float)window_h;
}
