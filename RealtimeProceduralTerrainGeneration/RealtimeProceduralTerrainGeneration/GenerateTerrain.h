#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include "Surf.h"
#include "Voronoi.h"
#include <glm/gtc/random.hpp>
#include "ThermalErosion.h"
#include "HydraulicErosion.h"
#define RESTART 99999

const int iterationTimes = 6;
const float width = 4.0f;
const float initialSpread = 1.0f;
const int randomPointsNum = 20;
const float magnitude = 0.25f;


GLuint create_surf_vao();
GLuint create_voronoi_vao();
GLuint create_terrain_vao(float ratio, bool bThermal, int tTime, bool bHydraulic, int hTime);
void DrawTerrain(GLuint vao);


void GenerateNewTerrain();
bool comp(const vec3 &a, const vec3 &b);
