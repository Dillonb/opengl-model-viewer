#include "render.h"
#include <gl.h>
#include <iostream>
#include "shader_vert.h"
#include "shader_frag.h"
#include "window.h"
#include "mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::unique_ptr;

namespace {
    unsigned int shaderProgram;
    unique_ptr<mesh> m;
}

void render_init() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* shader_vert_src_ptr = (const char*)&shader_vert_src;
    glShaderSource(vertexShader, 1, &shader_vert_src_ptr, (const GLint*)&shader_vert_src_len);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* shader_frag_src_ptr = (const char*)&shader_frag_src;
    glShaderSource(fragmentShader, 1, &shader_frag_src_ptr, (const GLint*)&shader_frag_src_len);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::cout << "Compiled and linked shaders" << std::endl;

    m = mesh::from_obj("/home/dillon/src/opengl-model-viewer/models/obj/cube.obj");
}

void render_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rendering code
    glUseProgram(shaderProgram);

    // Model transform
    static int spin = 0;
    static float scale = 0.1;
    m->transform = glm::mat4(1.0f);
    m->transform = glm::rotate(m->transform, glm::radians(float(spin)), glm::vec3(1.0, 1.0, 1.0));
    m->transform = glm::scale(m->transform, glm::vec3(scale, scale, scale));

    // Perspective transform
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), window_get_aspect_ratio(), 0.1f, 100.0f);

    glm::mat4 view(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, -10.0f, -10.0f));
    view = glm::lookAt(glm::vec3{0.0f, -2.0f, -2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f});

    m->render(shaderProgram, proj * view * m->transform);

    spin++;
    spin %= 360;

    static float scale_delta = 0.01;
    scale += scale_delta;
    if (scale > 1 || scale < 0.1) {
        scale_delta = -scale_delta;
    }
}
