#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include "../include/glad/gl.h"

GLuint load_shader_program(const char* vertex_path, const char* fragment_path);
char* read_file(const char* path);
GLuint compile_shader(const char* source, GLenum type);

#endif
