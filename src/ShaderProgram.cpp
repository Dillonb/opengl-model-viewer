#include "ShaderProgram.h"
#include <gl.h>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(const std::string &vertexShaderCode, const std::string &fragmentShaderCode) {
    const char* shader_vert_src_ptr = (const char*)vertexShaderCode.c_str();
    const char* shader_frag_src_ptr = (const char*)fragmentShaderCode.c_str();
    compile(shader_vert_src_ptr, vertexShaderCode.length(), shader_frag_src_ptr, fragmentShaderCode.length());
}

void ShaderProgram::compile(const char *vertexShaderCode, const size_t vertexShaderLen, const char *fragmentShaderCode, const size_t fragmentShaderLen) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, reinterpret_cast<const GLint*>(&vertexShaderLen));
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, reinterpret_cast<const GLint*>(&fragmentShaderLen));
    glCompileShader(fragmentShader);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::setM4(const char* name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(mat));
}
