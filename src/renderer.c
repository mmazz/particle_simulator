#include "../include/renderer.h"
#include "../include/particle.h"

static GLuint vao = 0;
GLuint vbo = 0;

void init_render(int num_particles) {
    glEnable(GL_PROGRAM_POINT_SIZE);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, num_particles * sizeof(Particle), NULL, GL_DYNAMIC_DRAW);

    // Posición (x, y) - location 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, x));

    // Color (r, g, b, a) - location 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, r));

    // Tamaño - location 2
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
}

void render_particles(Particle* particles, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Particle), particles);
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, count);
}

void cleanup_render() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

