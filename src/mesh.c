#include <mesh.h>
#include <allocator.h>

Mesh *newMesh(vec2s *vertices, vec2s *texCoords, GLuint *indices, uint32_t count)
{
    Mesh *mesh = ALLOCATE(Mesh, 1);

    mesh->vertices = vertices;
    mesh->texCoords = texCoords;
    mesh->indices = indices;
    mesh->count = count;

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO_vertices;
    glGenBuffers(1, &VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2s) * count, vertices, GL_STATIC_DRAW);

    GLuint VBO_texCoords;
    glGenBuffers(1, &VBO_texCoords);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_texCoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2s) * count, texCoords, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * count, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2s), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2s), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mesh->id = VAO;

    return mesh;
}
