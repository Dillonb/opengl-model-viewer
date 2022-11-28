#ifndef MODEL_VIEWER_SHADERPROGRAM_H
#define MODEL_VIEWER_SHADERPROGRAM_H

#include <string>
#include <glm/detail/type_mat4x4.hpp>

class ShaderProgram {
private:
    void compile(const char* vertexShaderCode, size_t vertexShaderLen,
                  const char* fragmentShaderCode, size_t fragmentShaderLen);
public:
    unsigned int id;
    ShaderProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
    ShaderProgram(const char* vertexShaderCode, size_t vertexShaderLen,
                  const char* fragmentShaderCode, size_t fragmentShaderLen) {
        compile(vertexShaderCode, vertexShaderLen, fragmentShaderCode, fragmentShaderLen) ;
    }

    void setM4(const char *string, const glm::mat4& mat);
};


#endif //MODEL_VIEWER_SHADERPROGRAM_H
