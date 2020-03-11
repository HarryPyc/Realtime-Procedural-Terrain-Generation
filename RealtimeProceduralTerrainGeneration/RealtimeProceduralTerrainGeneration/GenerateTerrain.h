#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include "Surf.h"
#include "Voronoi.h"
#define RESTART 99999

const int iterationTimes = 6;
const float width = 2.0f;
const float initialSpread = 0.5f;
const int randomPointsNum = 20;


GLuint create_surf_vao();
GLuint create_voronoi_vao();
GLuint create_terrain_vao();
void DrawTerrain(GLuint vao);
bool compZ(const vec3 &a, const vec3 &b);
bool compX(const vec3 &a, const vec3 &b);
