#include <iostream>
#include "window.h"
#include "render.h"
#include "mesh.h"

int main(int argc, char** argv) {
    window_init();
    render_init();


    while (!quit) {
        render_frame();
        window_frame();
    }
}