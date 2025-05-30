#ifndef RENDER_H
#define RENDER_H

#include "../include/glad/gl.h"
#include <stdlib.h>
#include "particle.h"
extern GLuint vbo;
void init_render(int num_particles);                       // Crea VAO/VBO
void render_particles(Particle* particles, int count);
void cleanup_render();                    // Libera buffers si querés limpiar al final

#endif

