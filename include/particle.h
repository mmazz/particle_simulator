#ifndef PARTICLE_H
#define PARTICLE_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
// Hay una mejor forma de iniciarlizaR?
// Y para dar valores iniciales? loop?
typedef struct {
    float x, y;
    float vx, vy;
    float r, g, b, a; // color
    float size;
    float life;       // vida restante
} Particle;

void update_particles(Particle* particles, int count, float dt);

void handle_collisions(Particle* particles, int count);
void influence_particles(float x_click, float y_click, Particle* particles, int count);
void add_particles(Particle** particles, int* count, int n_to_add);
#endif
