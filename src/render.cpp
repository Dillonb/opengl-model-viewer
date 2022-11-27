#include "render.h"
#include <gl.h>
#include <iostream>
#include "shader_vert.h"
#include "shader_frag.h"
#include "window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {
    unsigned int shaderProgram;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
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

    std::cout << "Compiled and linked shaders" << std::endl;

    /*
    float vertices[] = {
            // x    y     z     r     g     b
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,  // top left
             0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top middle
    };


    unsigned int indices[] = {
            1, 2, 4
            /*
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
             */ /*
    };
*/


    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // Loading code
    // Creates a single vertex array object, stores the handle to it in VAO
    glGenVertexArrays(1, &VAO);

    // Creates a single vertex buffer object
    glGenBuffers(1, &VBO);
    // Set it to an array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Attach it to the VAO we created earlier
    glBindVertexArray(VAO);
    // Load the vertex data from above in
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0); // enables vertex attrib array number zero

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // enables vertex attrib array number zero

    // Create an element buffer object to hold vertex indices
    //glGenBuffers(1, &EBO);
    // Set it to an element array buffer
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void render_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rendering code
    glUseProgram(shaderProgram);
    int transformLoc = glGetUniformLocation(shaderProgram, "transform");

    // Model transform
    static int spin = 0;
    static float scale = 0.1;
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(float(spin)), glm::vec3(1.0, 1.0, 1.0));
    model = glm::scale(model, glm::vec3(scale, scale, scale));

    // Perspective transform
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), window_get_aspect_ratio(), 0.1f, 100.0f);

    glm::mat4 view(1.0f);

    glm::mat4 mvp = proj * view * model;

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    spin++;
    spin %= 360;

    static float scale_delta = 0.01;
    scale += scale_delta;
    if (scale > 1 || scale < 0.1) {
        scale_delta = -scale_delta;
    }
}
