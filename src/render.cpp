#include "render.h"
#include <gl.h>
#include <iostream>
#include "shader_vert.h"
#include "shader_frag.h"
#include "window.h"
#include "mesh.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::unique_ptr;
using std::vector;

namespace {
    unique_ptr<ShaderProgram> shaderProgram;
    unique_ptr<mesh> m;
}

void render_init() {
    shaderProgram = std::make_unique<ShaderProgram>((const char*)shader_vert_src, shader_vert_src_len, (const char*)shader_frag_src, shader_frag_src_len);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::cout << "Compiled and linked shaders" << std::endl;

    //m = mesh::from_obj("/home/dillon/src/opengl-model-viewer/models/obj/head.obj");
    m = mesh::from_md2("/home/dillon/src/opengl-model-viewer/models/md2/Doomguy/doomguy.md2", "/home/dillon/src/opengl-model-viewer/models/md2/Doomguy/green.png");

    //meshes = mesh::from_md2("/home/dillon/src/opengl-model-viewer/models/md2/Monsters/Cyberdemon/cyberdemon.MD2", "/home/dillon/src/opengl-model-viewer/models/md2/Monsters/Cyberdemon/Cyberdemon.png");
}

void render_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Model transform
    static int spin = 0;
    static float scale = 1;
    static int frame = 0;
    static int frame_switch = 0;
    frame_switch = (frame_switch + 1) % 10;
    if (frame_switch == 0) {
        frame = (frame + 1) % m->frames.size();
    }

    m->transform = glm::mat4(1.0f);
    m->transform = glm::rotate(m->transform, glm::radians(float(spin)), glm::vec3(0.0, 0.0, 1.0));
    m->transform = glm::scale(m->transform, glm::vec3(scale, scale, scale));

    // Perspective transform
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), window_get_aspect_ratio(), 0.1f, 10000000.0f);

    // Eye position
    glm::mat4 view(1.0f);
    view = glm::lookAt(
            // Where we are
            glm::vec3{100.0f, 0.0f, 0.0f},
            // Where we are looking at
            {0.0f, 0.0f, 0.0f},
            // Where the fuck is up
            {0.0f, 0.0f, 1.0f});

    m->render(*shaderProgram, proj * view * m->transform, frame);

    spin++;
    spin %= 360;

    /*
    static float scale_delta = 0.01;
    scale += scale_delta;
    if (scale > 2 || scale < 1) {
        scale_delta = -scale_delta;
    }
     */
}
