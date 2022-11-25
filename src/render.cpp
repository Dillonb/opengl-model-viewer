#include "render.h"
#include <gl.h>
#include <iostream>
#include "shader_vert.h"
#include "shader_frag.h"

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

    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };

    // Loading code
    // Creates a single vertex array object, stores the handle to it in VAO
    glGenVertexArrays(1, &VAO);

    // Creates a single vertex buffer object
    glGenBuffers(1, &VBO);
    // Set it to an array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Attach it to the VAO we created earlier
    glBindVertexArray(VAO);
    // Load the vertex data from above in
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0); // enables vertex attrib array number zero

    // Create an element buffer object to hold vertex indices
    glGenBuffers(1, &EBO);
    // Set it to an element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void render_frame() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Rendering code
    glUseProgram(shaderProgram);
    //glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


}
