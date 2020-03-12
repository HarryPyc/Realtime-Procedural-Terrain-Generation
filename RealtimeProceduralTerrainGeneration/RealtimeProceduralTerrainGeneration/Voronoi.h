#pragma once
#include "Mathfunc.h"

class Voronoi
{	
public:
	vector<vec3> v;
	vector<vec3> rpoints;
	int N;
	float c1, c2;
	float d1, d2;
	void GetD1D2(vec3 p);
	Voronoi(int times, int rNum, float c1, float c2, float width);
	Voronoi();
	~Voronoi();
};

