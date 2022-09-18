#pragma once

#include "../render_item/render_item.h"

void bind_vao(GLuint *vao);
void bind_vbo(GLuint *vbo);
void bind_ebo(GLuint *ebo);
void unbind_vao(Render_Item *render_item);
void unbind_vbo(Render_Item *render_item);
void unbind_ebo(Render_Item *render_item);
void generate_vao(Render_Item *render_item);
void generate_vbo(f32 *vertices, int length, u32 *vbo);
void generate_dynamic_vbo(f32 *vertices, int length, u32 *vbo);
void generate_ebo(u32 *indices, int length, u32 *ebo);
void link_attributes(Render_Item *render_item);