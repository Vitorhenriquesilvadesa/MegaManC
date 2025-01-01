#ifndef MESH_H
#define MESH_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <glad/glad.h>
#include <stdint.h>

typedef struct
{
    uint32_t count;
    vec2s *vertices;
    vec2s *texCoords;
    GLuint *indices;
    GLuint id;
} Mesh;

Mesh *newMesh(vec2s *vertices, vec2s *texCoords, GLuint *indices, uint32_t count);
void bindMesh(Mesh *mesh);
void unbindMeshes();
void drawMesh(Mesh *mesh);
void drawMeshWireframe(Mesh *mesh);

#endif