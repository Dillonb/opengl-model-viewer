#include "mesh.h"
#include "md2_types.h"

#include <cstring>
#include <vector>
#include <gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::string;

#define streql(x, y) (strcmp(x, y) == 0)

md2_vec3_t anorms_table[162] = {
#include "anorms.h"
};

mesh::mesh(const vector<vertex>& packed_vertices) {
    VAO = 0;
    VBO = 0;
    num_vertices = packed_vertices.size();

    glGenVertexArrays(1, &VAO);

    // Creates a single vertices buffer object
    glGenBuffers(1, &VBO);
    // Set it to an array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Attach it to the VAO we created earlier
    glBindVertexArray(VAO);
    // Load the vertices data from above in
    glBufferData(GL_ARRAY_BUFFER, packed_vertices.size() * sizeof(vertex), packed_vertices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
    glEnableVertexAttribArray(0); // enables vertices attrib array number zero

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(1); // enables vertices attrib array number zero

    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));
    glEnableVertexAttribArray(2); // enables vertices attrib array number zero
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
vector<unique_ptr<mesh>> mesh::from_md2(const string& filename) {
    FILE* fp = fopen(filename.c_str(), "rb");
    md2_header_t h = {0};
    fread(&h, sizeof(md2_header_t), 1, fp);

    vector<md2_skin_t> skins(h.num_skins);
    skins.resize(h.num_skins);
    fseek(fp, h.offset_skins, SEEK_SET);
    fread(skins.data(), sizeof(md2_skin_t), h.num_skins, fp);

    vector<md2_texCoord_t> uv(h.num_st);
    uv.resize(h.num_st);
    fseek(fp, h.offset_st, SEEK_SET);
    fread(uv.data(), sizeof(md2_texCoord_t), h.num_st, fp);

    vector<md2_triangle_t> triangles(h.num_tris);
    triangles.resize(h.num_tris);
    fseek(fp, h.offset_tris, SEEK_SET);
    fread(triangles.data(), sizeof(md2_triangle_t), h.num_tris, fp);

    vector<int> glcmds(h.num_glcmds);
    glcmds.resize(h.num_glcmds);
    fseek(fp, h.offset_glcmds, SEEK_SET);
    fread(glcmds.data(), sizeof(int), h.num_glcmds, fp);

    vector<unique_ptr<mesh>> meshes;
    meshes.reserve(h.num_frames);

    fseek(fp, h.offset_frames, SEEK_SET);
    for (int i = 0; i < h.num_frames; i++) {
        md2_frame_t frame;
        fread(frame.scale, sizeof(md2_vec3_t), 1, fp);
        fread(frame.translate, sizeof(md2_vec3_t), 1, fp);
        fread(frame.name, sizeof(char), 16, fp);

        frame.verts.resize(h.num_vertices);
        fread(frame.verts.data(), sizeof(md2_vertex_t), h.num_vertices, fp);

        vector<vertex> packed_vertices;
        for (const auto& triangle : triangles) {
            for (const auto& vertex_idx : triangle.vertices) {
                auto vertex = frame.verts[vertex_idx];
                glm::vec3 pos(
                        (frame.scale[0] * (float)vertex.v[0]) + frame.translate[0],
                        (frame.scale[1] * (float)vertex.v[1]) + frame.translate[1],
                        (frame.scale[2] * (float)vertex.v[2]) + frame.translate[2]
                );
                /*
                s = mdl->texcoords[mdl->triangles[i].st[j]].s / mdl->header.skinwidth;
                t = mdl->texcoords[mdl->triangles[i].st[j]].t / mdl->header.skinheight;
                 */
                auto anorm = anorms_table[vertex.normalIndex];
                packed_vertices.emplace_back(pos, glm::vec3(anorm[0], anorm[1], anorm[2]), glm::vec2(0, 0));
            }
        }

        meshes.emplace_back(new mesh(packed_vertices));
    }

    return std::move(meshes);
}

void mesh::render(unsigned int shaderProgram, const glm::mat4 &mvp) const {
    int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}
