#include "Voronoi.h"


void Voronoi::GetD1D2(vec3 p)
{
	vector<float> dist;
	vector<vec3>::iterator it = rpoints.begin();
	for (it; it < rpoints.end(); it++) {
		dist.push_back(distance(*it, p));
	}
	sort(dist.begin(), dist.end());
	d1 = dist[0];
	d2 = dist[1];
}

Voronoi::Voronoi(int times, int rNum, float c1, float c2) {
	this->c1 = c1;
	this->c2 = c2;
	for (int i = 0; i < rNum; i++) {
		vec3 r(random11(), 0.f, random11());
		rpoints.push_back(r);
	}
	N = pow(2, times) + 1;
	float offset = 2.f / (N - 1);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			float x = j * offset - 1.0f;
			float z = i * offset - 1.0f;
			vec3 p(x, 0.f, z);
			GetD1D2(p);
			p.y = c1 * d1 + c2 * d2;
			v.push_back(p);
		}
	}
}
Voronoi::Voronoi()
{
}


Voronoi::~Voronoi()
{
}
