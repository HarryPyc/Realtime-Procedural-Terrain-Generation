#include "Voronoi.h"

//Get the 1st and 2nd nearest distance
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

Voronoi::Voronoi(int times, int rNum, float c1, float c2, float width) {
	this->c1 = c1;
	this->c2 = c2;
	float w = width / 2.f;
#pragma omp for
	//generate random points
	for (int i = 0; i < rNum; i++) {
		vec3 r(random11()*w, 0.f, random11()*w);
		rpoints.push_back(r);
	}
	N = pow(2, times) + 1;
	float offset = width / (N - 1);
#pragma omp for
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			float x = j * offset - w;
			float z = i * offset - w;
			vec3 p(x, 0.f, z);
			GetD1D2(p);
			//get voronoi map
			p.y = c1 * d1 + c2 * d2;
			v.push_back(p);
		}
	}
}



Voronoi::~Voronoi()
{
	
}
