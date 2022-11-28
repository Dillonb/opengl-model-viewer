#include "mesh.h"
#include "md2_types.h"

#include <cstring>
#include <vector>
#include <gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::string;

#define streql(x, y) (strcmp(x, y) == 0)

glm::vec3 anorms_table[162] = {
#include "anorms.h"
};

mesh::mesh(const vector<vertex>& packed_vertices, unsigned int gl_tex_id)
        : gl_tex_id(gl_tex_id) {
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
vector<unique_ptr<mesh>> mesh::from_md2(const string& filename, const string& texture_filename) {
    int texture_width, texture_height, texture_components;
    uint8_t* texture_data = nullptr;
    unsigned int gl_tex_id = 0;
    if (!texture_filename.empty()) {
        texture_data = stbi_load(texture_filename.c_str(), &texture_width, &texture_height, &texture_components, STBI_rgb_alpha);
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &gl_tex_id);
        glBindTexture(GL_TEXTURE_2D, gl_tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        printf("Loading texture: %p w: %d h: %d\n", texture_data, texture_width, texture_height);
    }

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
    for (int frame_idx = 0; frame_idx < h.num_frames; frame_idx++) {
        md2_frame_t frame;
        fread(&frame.scale, sizeof(glm::vec3), 1, fp);
        fread(&frame.translate, sizeof(glm::vec3), 1, fp);
        fread(frame.name, sizeof(char), 16, fp);

        frame.verts.resize(h.num_vertices);
        fread(frame.verts.data(), sizeof(md2_vertex_t), h.num_vertices, fp);

        vector<vertex> packed_vertices;
        for (const auto& triangle : triangles) {
            for (int i = 0; i < 3; i++) {
                int vertex_idx = triangle.vertices[i];
                md2_vertex_t vertex = frame.verts[vertex_idx];

                glm::vec3 pos(
                        (frame.scale[0] * (float)vertex.v[0]) + frame.translate[0],
                        (frame.scale[1] * (float)vertex.v[1]) + frame.translate[1],
                        (frame.scale[2] * (float)vertex.v[2]) + frame.translate[2]
                );

                packed_vertices.emplace_back(
                        pos,
                        anorms_table[vertex.normalIndex],
                        glm::vec2(
                                (float)uv[triangle.st[i]].s / h.skinwidth,
                                (float)uv[triangle.st[i]].t / h.skinheight));
            }
        }

        meshes.emplace_back(new mesh(packed_vertices, gl_tex_id));
    }

    return meshes;
}

void mesh::render(unsigned int shaderProgram, const glm::mat4 &mvp) const {
    int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    /*
    int samplerLoc = glGetUniformLocation(shaderProgram, "sampler");
    glUniform1i(samplerLoc, 0);
    printf("Sampler loc: %d\n", samplerLoc);
     */
    if (gl_tex_id > 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gl_tex_id);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

}
