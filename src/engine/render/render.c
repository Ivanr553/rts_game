#include <glad/glad.h>
#include <ft2build.h>
#include <stb/stb_image.h>
#include <linmath.h>
#include <stdlib.h>
#include <string.h>

#include "../global.h"
#include "render.h"
#include "render_internal.h"
#include "../util/util.h"
#include "../../generic/generic.h"
#include "../entity/entity.h"
#include "../../game/state/state.h"

void render_begin(void)
{
    global.render.tick++;
    if (global.render.tick == 61)
    {
        global.render.tick = 1;
    }
    glClearColor(0.08, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void render_end(void)
{
    SDL_GL_SwapWindow(global.render.window);
};

void use_shader(Render_Item *render_item, int shader_id)
{
    if (!render_item)
    {
        ERROR_EXIT("No render_item provided to use_shader\n");
    }

    vec4 light_color = {1, 1, 1, 1};
    vec3 light_pos = {10, 10, 400};

    u32 default_shader_id = global.render.shader_groups[0].shader_id;
    u32 light_shader_id = global.render.shader_groups[1].shader_id;
    u32 color_shader_id = global.render.shader_groups[2].shader_id;
    if (shader_id == default_shader_id)
    {
        if (render_item->should_ignore_camera)
        {
            mat4x4 default_camera;
            mat4x4_identity(default_camera);
            glUniformMatrix4fv(glGetUniformLocation(shader_id, "camera"), 1, GL_FALSE, &default_camera[0][0]);
        }
        else
        {
            glUniformMatrix4fv(glGetUniformLocation(shader_id, "camera"), 1, GL_FALSE, &global.camera.camera_uniform[0][0]);
        }
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "model"), 1, GL_FALSE, &render_item->model[0][0]);
        glUniform4f(glGetUniformLocation(shader_id, "light_color"), light_color[0], light_color[1], light_color[2], light_color[3]);
        glUniform3f(glGetUniformLocation(shader_id, "light_pos"), light_pos[0], light_pos[1], light_pos[2]);
        glUniform1i(glGetUniformLocation(shader_id, "texture_id"), 0);
        glUniform1f(glGetUniformLocation(shader_id, "scale"), 1);
    }
    else if (shader_id == light_shader_id)
    {
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "model"), 1, GL_FALSE, &render_item->model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "camera"), 1, GL_FALSE, &global.camera.camera_uniform[0][0]);
        glUniform4f(glGetUniformLocation(shader_id, "light_color"), light_color[0], light_color[1], light_color[2], light_color[3]);
    }
    else if (shader_id == color_shader_id)
    {
        if (render_item->should_ignore_camera)
        {
            mat4x4 default_camera;
            mat4x4_identity(default_camera);
            glUniformMatrix4fv(glGetUniformLocation(shader_id, "camera"), 1, GL_FALSE, &default_camera[0][0]);
        }
        else
        {
            glUniformMatrix4fv(glGetUniformLocation(shader_id, "camera"), 1, GL_FALSE, &global.camera.camera_uniform[0][0]);
        }
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "model"), 1, GL_FALSE, &render_item->model[0][0]);
        glUniform4f(glGetUniformLocation(shader_id, "color"), render_item->color[0], render_item->color[1], render_item->color[2], render_item->color[3]);
        glUniform1f(glGetUniformLocation(shader_id, "scale"), 1);
    }
    else
    {
        if (render_item->should_ignore_camera)
        {
            mat4x4 default_camera;
            mat4x4_identity(default_camera);
            glUniformMatrix4fv(glGetUniformLocation(default_shader_id, "camera"), 1, GL_FALSE, &default_camera[0][0]);
        }
        else
        {
            glUniformMatrix4fv(glGetUniformLocation(default_shader_id, "camera"), 1, GL_FALSE, &global.camera.camera_uniform[0][0]);
        }
        glUniformMatrix4fv(glGetUniformLocation(default_shader_id, "model"), 1, GL_FALSE, &render_item->model[0][0]);
        glUniform4f(glGetUniformLocation(default_shader_id, "light_color"), light_color[0], light_color[1], light_color[2], light_color[3]);
        glUniform3f(glGetUniformLocation(default_shader_id, "light_pos"), light_pos[0], light_pos[1], light_pos[2]);
        glUniform1i(glGetUniformLocation(default_shader_id, "texture_id"), 0);
        glUniform1f(glGetUniformLocation(default_shader_id, "scale"), 1);
    }
};

void render(void)
{

    // printf("Starting render\n");

    // Initialization
    camera_matrix(CAMERA_FOV, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);

    Shader_Group *shader_groups = global.render.shader_groups;

    for (int s = 0; s < SUPPORTED_SHADER_COUNT; s++)
    {
        Shader_Group shader_group = shader_groups[s];
        glUseProgram(shader_group.shader_id);

        Texture_Group *texture_groups = shader_group.texture_groups;

        for (int t = 0; t < SUPPORTED_TEXTURE_COUNT; t++)
        {
            Texture_Group texture_group = texture_groups[t];

            glBindTexture(GL_TEXTURE_2D, texture_group.texture_id);

            Array *render_items = texture_group.render_items;

            for (int i = 0; i < render_items->len; i++)
            {
                Render_Item *render_item_ptr = get_item_from_array(render_items, i);
                Render_Item render_item = *render_item_ptr;

                if (render_item.hide)
                {
                    continue;
                }

                // printf("Rendering render_item: %d\n", render_item.id);

                // mat4x4_translate(item->model, -item->pos[0], -item->pos[1], -item->pos[2]);
                // rotate_model(render_item.model, render_item.rotation);

                scale_model(&render_item.model, render_item.size, render_item.should_ignore_camera);
                // mat4x4_scale_aniso(item->model, item->model, 2, 2, 1);

                if (render_item.texture_data->texture_location != 0)
                {
                    glBindTexture(GL_TEXTURE_2D, *render_item.texture_data->texture);
                }

                bind_vao(render_item.vao);
                // printf("Finished binding the vao\n");
                bind_vbo(render_item.vbo);
                use_shader(&render_item, shader_group.shader_id);
                // printf("Finished using the shader\n");

                if (render_item.updated == 1)
                {
                    glBufferSubData(GL_ARRAY_BUFFER, 0, render_item.vertices_len * sizeof(f32), render_item.vertices);
                    render_item_ptr->updated = 0;
                }

                if (render_item.indices_len == 0)
                {
                    // printf("Vertices_len: %d\n", render_item.vertices_len);
                    glLineWidth(3);
                    glDrawArrays(GL_LINE_LOOP, 0, render_item.vertices_len / render_item.vertex_stride);
                }
                else
                {
                    glDrawElements(GL_TRIANGLES, render_item.indices_len, GL_UNSIGNED_INT, NULL);
                }

                glBindTexture(GL_TEXTURE_2D, 0);
                unbind_vbo(&render_item);
                unbind_vao(&render_item);

                GLenum error = glGetError();
                if (error)
                {
                    printf("Got error: %d %x\n", i, error);
                }

                if (render_item.cleanup)
                {
                    // printf("Cleaning up render item\n");
                    // remove_item_from_array(render_items, i);
                }
                // printf("Render item texture file path: %s\n", render_item.texture_data->file_path);
            }
        }
    }
}