#pragma once

#include "../../types.h"

#include "../../util/constants.h"
#include "../render_item/render_item.h"

void render_text_in_world(char *c, vec3 pos);
Render_Item *render_text_on_screen(Render_Item *render_item, char *c, int max_str_len, vec3 pos, vec2 size);
void update_text_item(Render_Item *render_item, char *c, vec3 pos, vec2 size);