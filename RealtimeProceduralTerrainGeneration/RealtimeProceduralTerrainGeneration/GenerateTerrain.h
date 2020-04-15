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

class Terrain {
public:
	vector<vec3> v;
	int N;
	float hMax = -999.f;
	float hMin = 999.f;
	Terrain();
	void thermal(int time);
	void hydraulic(int time);
	void turb(float c = 1.5f);
	
	void init();
	void update(float ratio = 0.667f, bool bThermal = true, int tTime = 50, bool bHydraulic = true, int hTime = 50, float c = 1.5f);
private:
	Surf* surf;
	Voronoi* vor;
};


GLuint create_terrain_vao(vector<vec3> *v, int N);
void DrawTerrain(GLuint vao, int N);



bool comp(const vec3 &a, const vec3 &b);
