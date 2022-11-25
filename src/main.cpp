#include <iostream>
#include "window.h"
#include "render.h"

int main(int argc, char** argv) {
    std::cout << "hello" << std::endl;
    window_init();
    render_init();

    while (!quit) {
        render_frame();
        window_frame();
    }
}