#ifndef MODEL_VIEWER_MESH_H
#define MODEL_VIEWER_MESH_H

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

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
    mesh(const std::vector<vertex>& packed_vertices);
public:
    static std::unique_ptr<mesh> from_obj(const std::string& filename);
    static std::vector<std::unique_ptr<mesh>> from_md2(const std::string& filename);
    void render(unsigned int shaderProgram, const glm::mat4& mvp) const;
    unsigned int VBO;
    unsigned int VAO;
    int num_vertices;
    glm::mat4 transform;
};


#endif //MODEL_VIEWER_MESH_H
