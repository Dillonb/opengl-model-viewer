#include "mesh.h"

#include <cstring>
#include <vector>
#include <gl.h>

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::string;

#define streql(x, y) (strcmp(x, y) == 0)

mesh::mesh(const vector<vertex>& packed_vertices) {
    VAO = 0;
    VBO = 0;
    num_vertices = packed_vertices.size();

    glGenVertexArrays(1, &VAO);

    // Creates a single vertex buffer object
    glGenBuffers(1, &VBO);
    // Set it to an array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Attach it to the VAO we created earlier
    glBindVertexArray(VAO);
    // Load the vertex data from above in
    glBufferData(GL_ARRAY_BUFFER, packed_vertices.size() * sizeof(vertex), packed_vertices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
    glEnableVertexAttribArray(0); // enables vertex attrib array number zero

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(1); // enables vertex attrib array number zero

    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));
    glEnableVertexAttribArray(2); // enables vertex attrib array number zero
}

unique_ptr<mesh> mesh::from_obj(const string& filename) {
    FILE* f = fopen(filename.c_str(), "r");
    char* line = nullptr;
    size_t len;

    vector<glm::vec3> obj_pos;
    vector<glm::vec2> obj_uv;
    vector<glm::vec3> obj_normal;

    vector<vertex> packed_vertices;

    while (getline(&line, &len, f) != -1) {
        char* tok = strtok(line, " ");

        if (streql(tok, "v")) {
            glm::vec3 position;
            for (int i = 0; i < 3; i++) {
                tok = strtok(nullptr, " ");
                position[i] = std::strtof(tok, nullptr);
            }
            obj_pos.emplace_back(position);
        } else if (streql(tok, "vt")) {
            glm::vec2 uv;
            for (int i = 0; i < 2; i++) {
                tok = strtok(nullptr, " ");
                uv[i] = std::strtof(tok, nullptr);
            }
            obj_uv.emplace_back(uv);
        } else if (streql(tok, "vn")) {
            glm::vec3 normal;
            for (int i = 0; i < 3; i++) {
                tok = strtok(nullptr, " ");
                normal[i] = std::strtof(tok, nullptr);
            }
            obj_normal.emplace_back(normal);
        } else if (streql(tok, "f")) {
            // The spec states that all vertices, texture coords, and normals
            // must exist before being added to a face
            for (int i = 0; i < 3; i++) {
                tok = strtok(nullptr, " ");
                int pos_idx = 0, uv_idx = 0, normal_idx = 0;
                sscanf(tok, "%d/%d/%d", &pos_idx, &uv_idx, &normal_idx);

                if (pos_idx < 0) {
                    pos_idx += obj_pos.size();
                }
                pos_idx--;

                if (uv_idx < 0) {
                    uv_idx += obj_uv.size();
                }
                uv_idx--;

                if (normal_idx < 0) {
                    normal_idx += obj_normal.size();
                }
                normal_idx--;

                packed_vertices.emplace_back(obj_pos[pos_idx], obj_normal[normal_idx], obj_uv[uv_idx]);
            }
        }
    }

    return unique_ptr<mesh>(new mesh(packed_vertices));
}
