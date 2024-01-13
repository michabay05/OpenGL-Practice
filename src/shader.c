#include <glad/glad.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "shader.h"
#include "util.h"

bool shader_load(Shader* shader, const char* vertex_path, const char* fragment_path)
{
    bool compiled = true;

    // Read vertex shader source from file path and compile it
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    // TODO: handle 'null' cases
    const char* vertex_shader_source = read_entire_file(vertex_path);
    glShaderSource(vertex_shader_id, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader_id);

    // Check if compilation is successful
    GLint success;
    char info_log[512] = {0};
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        compiled = false;
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        fprintf(stderr, "[ERROR] Failed to compile vertex shader '%s'.\n%s\n", vertex_path, info_log);
    } else {
        printf("[INFO] Compiled vertex shader: '%s'.\n", vertex_path);
    }
    free((char*)vertex_shader_source);

    // Read fragment shader source from file path and compile it
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    // TODO: handle 'null' cases
    const char* fragment_shader_source = read_entire_file(fragment_path);
    glShaderSource(fragment_shader_id, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader_id);

    // Check if compilation was successful
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        compiled = false;
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
        fprintf(stderr, "[ERROR] Failed to compile fragment shader '%s'.\n%s\n", fragment_path, info_log);
    } else {
        printf("[INFO] Compiled fragment shader: '%s'.\n", fragment_path);
    }
    free((char*)fragment_shader_source);

    // Create a shader program to attach the shader ids to
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader_id);
    glAttachShader(shader_program, fragment_shader_id);
    glLinkProgram(shader_program);

    // Check if linking shader was successful
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        compiled = false;
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, "[ERROR] Failed to link program.\n");
    }
    shader->id = shader_program;

    // Delete the created shaders
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return compiled;
}
void shader_use(Shader shader)
{
    glUseProgram(shader.id);
}

void shader_set_float(Shader shader, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shader.id, name), value);
}

void shader_set_int(Shader shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader.id, name), value);
}

void shader_set_mat4(Shader shader, const char* name, const float* value)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.id, name), 1, GL_FALSE, value);
}
