#ifndef MODEL_VIEWER_MESH_H
#define MODEL_VIEWER_MESH_H

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "ShaderProgram.h"

class mesh {
private:
    struct vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;

        vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv)
                : position(position),
                  normal(normal),
                  uv(uv) {}
    };
    struct frame {
        std::string name;

        unsigned int VBO;
        unsigned int VAO;
        unsigned int gl_tex_id;
        int num_vertices;

        explicit frame(const std::vector<vertex>& packed_vertices, const std::string& name = "", unsigned int gl_tex_id = 0);
    };
public:
    explicit mesh(std::vector<frame> frames) : frames(std::move(frames)), transform(1.0f) {}
    static std::unique_ptr<mesh> from_obj(const std::string& filename);
    static std::unique_ptr<mesh> from_md2(const std::string& filename, const std::string& texture_filename);
    void render(ShaderProgram& shaderProgram, const glm::mat4& mvp, int frame = 0) const;
    std::vector<frame> frames;
    glm::mat4 transform;
};


#endif //MODEL_VIEWER_MESH_H
