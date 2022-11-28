#ifndef MODEL_VIEWER_MD2_TYPES_H
#define MODEL_VIEWER_MD2_TYPES_H

#include <vector>

struct md2_header_t {
    int ident;                  /* magic number: "IDP2" */
    int version;                /* version: must be 8 */

    int skinwidth;              /* texture width */
    int skinheight;             /* texture height */

    int framesize;              /* size in bytes of a frame */

    int num_skins;              /* number of skins */
    int num_vertices;           /* number of vertices per frame */
    int num_st;                 /* number of texture coordinates */
    int num_tris;               /* number of triangles */
    int num_glcmds;             /* number of opengl commands */
    int num_frames;             /* number of frames */

    int offset_skins;           /* offset skin data */
    int offset_st;              /* offset texture coordinate data */
    int offset_tris;            /* offset triangle data */
    int offset_frames;          /* offset frame data */
    int offset_glcmds;          /* offset OpenGL command data */
    int offset_end;             /* offset end of file */
};

/* Vector */
typedef float md2_vec3_t[3];

/* Texture name */
struct md2_skin_t {
    char name[64];              /* texture file name */
};

/* Texture coords */
struct md2_texCoord_t
{
    short s;
    short t;
};

/* Triangle info */
struct md2_triangle_t
{
    unsigned short vertices[3];   /* vertices indices of the triangle */
    unsigned short st[3];       /* tex. coord. indices */
};

/* Compressed vertices */
struct md2_vertex_t
{
    unsigned char v[3];         /* position */
    unsigned char normalIndex;  /* normal vector index */
};

/* Model frame */
struct md2_frame_t
{
    md2_vec3_t scale;               /* scale factor */
    md2_vec3_t translate;           /* translation vector */
    char name[16];              /* frame name */
    std::vector<md2_vertex_t> verts; /* list of frame's vertices */
    //struct md2_vertex_t *verts;
};

#endif //MODEL_VIEWER_MD2_TYPES_H
