#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include "Surf.h"
#include "Voronoi.h"
#include <glm/gtc/random.hpp>
#include "ThermalErosion.h"
#include "HydraulicErosion.h"
#define RESTART 99999

const int iterationTimes = 10;
const float width = 16.0f;
const float initialSpread = 4.0f;
const int randomPointsNum = 20;


class Terrain {
public:
	vector<vec3> v;
	int N;
	float hMax = -999.f;
	float hMin = 999.f;
	int Tcount = 0, Hcount = 0;
	Terrain(float ratio, bool bThermal, int tTime, bool bHydraulic, int hTime);
	~Terrain();
	void thermal(int time);
	void hydraulic(int time);
	void turb(float c = 1.5f);
	
	void init();
	void update(float ratio, bool bThermal, int tTime, bool bHydraulic, int hTime);
private:
	Surf* surf;
	Voronoi* vor;
};


GLuint create_terrain_vao(vector<vec3> *v, int N);
void DrawTerrain(GLuint vao, int N);



bool comp(const vec3 &a, const vec3 &b);
