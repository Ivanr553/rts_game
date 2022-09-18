#include <stdlib.h>
#include <glad/glad.h>

#include "opengl_objects.h"

/* ================= BINDING ================= */

void bind_vao(GLuint *vao)
{
    if (!vao)
    {
        ERROR_EXIT("No vao provided to bind_vao\n");
    }
    glBindVertexArray(*vao);
};

void bind_vbo(GLuint *vbo)
{
    if (!vbo)
    {
        ERROR_EXIT("No vbo provided to bind_vbo\n");
    }

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
};

void bind_ebo(GLuint *ebo)
{
    if (!ebo)
    {
        ERROR_EXIT("No ebo provided to bind_ebo\n");
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
};

/* ================= UNBINDING ================= */

void unbind_vao(Render_Item *render_item)
{
    if (!render_item)
    {
        ERROR_EXIT("No render_item provided to unbind_vao\n");
    }

    glBindVertexArray(0);
};

void unbind_vbo(Render_Item *render_item)
{
    if (!render_item)
    {
        ERROR_EXIT("No render_item provided to unbind_vbo\n");
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
};

void unbind_ebo(Render_Item *render_item)
{
    if (!render_item)
    {
        ERROR_EXIT("No render_item provided to unbind_ebo\n");
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};

/* ================= GENERATING ================= */

void generate_vao(Render_Item *render_item)
{
    if (!render_item)
    {
        ERROR_EXIT("No render_item provided to generate_vao\n");
    }

    glGenVertexArrays(1, render_item->vao);
};

void generate_vbo(f32 *vertices, int length, u32 *vbo)
{
    glGenBuffers(1, vbo);
    // printf("Generated vbo buffer\n");
    bind_vbo(vbo);
    // printf("Bound vbo\n");
    // printf("Generating buffer of size: %d\n", length * sizeof(f32));
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(f32), vertices, GL_STATIC_DRAW);
    // printf("Added data to array buffer\n");
};

void generate_dynamic_vbo(f32 *vertices, int length, u32 *vbo)
{
    glGenBuffers(1, vbo);
    // printf("Generated vbo buffer\n");
    bind_vbo(vbo);
    // printf("Bound vbo\n");
    // printf("Generating dynamic array buffer of size: %d\n", length * sizeof(f32));
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(f32), vertices, GL_DYNAMIC_DRAW);
    // printf("Added data to array buffer\n");
};

void generate_ebo(u32 *indices, int length, u32 *ebo)
{
    if (length == 0)
    {
        printf("Not generating ebo, no indices length\n");
        return;
    }

    glGenBuffers(1, ebo);
    // printf("Generated ebo\n");
    bind_ebo(ebo);
    // printf("Bound ebo\n");
    // printf("Generating dynamic element array buffer of size: %d\n", length * sizeof(u32));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(u32), indices, GL_STATIC_DRAW);
    // printf("Added data to element array buffer\n");
};

/* ================= LINK / EXECUTE ================= */

void link_attributes(Render_Item *render_item)
{
    if (!render_item)
    {
        ERROR_EXIT("No render_item provided to generate_ebo\n");
    }

    // printf("Starting link attributes\n");
    if (render_item->shader_position == 0)
    {
        // Assigning values for location
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), NULL);

        // assigning coords for the texture uvs
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), (void *)(3 * sizeof(f32)));

        // assigning color for the shader
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), (void *)(5 * sizeof(f32)));

        // normals for light face calculations
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), (void *)(9 * sizeof(f32)));
    }
    else if (render_item->shader_position == 1)
    {
        // Assigning values for location
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), NULL);
    }
    else if (render_item->shader_position == 2)
    {
        // Assigning values for location
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), NULL);

        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), (void *)(3 * sizeof(f32)));
    }
    else
    {
        // Assigning values for location
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), NULL);

        // assigning coords for the texture uvs
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), (void *)(3 * sizeof(f32)));

        // assigning color for the shader
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), (void *)(5 * sizeof(f32)));

        // normals for light face calculations
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, render_item->vertex_stride * sizeof(f32), (void *)(9 * sizeof(f32)));
    }
    // printf("Finished linking attributes\n");
    unbind_vao(render_item);
    unbind_vbo(render_item);
    // printf("Finished unbinding vbo\n");
};
