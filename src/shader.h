#pragma once

typedef struct {
    int id;
} Shader;

bool shader_load(Shader* shader, const char* vertex_path, const char* fragment_path);
void shader_use(Shader shader);
void shader_set_float(Shader shader, const char* name, float value);
void shader_set_int(Shader shader, const char* name, int value);
void shader_set_mat4(Shader shader, const char* name, const float* value);
