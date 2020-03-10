#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include "Surf.h"

const int iterationTimes = 5;
const float width = 2.0f;
const float initialSpread = 1.0f;


GLuint create_surf_vao();
void DrawTerrain(GLuint vao);
