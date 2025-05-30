#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/shader.h"

char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Error al abrir archivo: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "Error de memoria leyendo archivo: %s\n", path);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);
    return buffer;
}

GLuint compile_shader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        fprintf(stderr, "Error compilando shader (%s):\n%s\n",
                (type == GL_VERTEX_SHADER ? "vértice" : "fragmento"), log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint load_shader_program(const char* vertex_path, const char* fragment_path) {
    char* vertex_code = read_file(vertex_path);
    char* fragment_code = read_file(fragment_path);
    if (!vertex_code || !fragment_code) {
        free(vertex_code);
        free(fragment_code);
        return 0;
    }

    GLuint vertex_shader = compile_shader(vertex_code, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_code, GL_FRAGMENT_SHADER);
    free(vertex_code);
    free(fragment_code);

    if (!vertex_shader || !fragment_shader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        fprintf(stderr, "Error vinculando shader program:\n%s\n", log);
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}


