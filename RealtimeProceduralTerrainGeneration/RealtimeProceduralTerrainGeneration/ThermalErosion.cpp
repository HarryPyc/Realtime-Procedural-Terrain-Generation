#include "ThermalErosion.h"
float getNewHeight(float h, float d, float dMax, float dTotal, float T) {
	const float c = 0.5f;
	if (d > T) {
		return h + c * (dMax - T) * d / dTotal;
	}
	else {
		return h;
	}
}
void iteration(vector<vec3> *v, int i, int j, int N) {
	float h = v->at((i)*N + j).y;
	//get 4 neighbors
	float h1 = v->at((i - 1) * N + j - 1).y;
	float h2 = v->at((i - 1) * N + j + 1).y;
	float h3 = v->at((i + 1) * N + j - 1).y;
	float h4 = v->at((i + 1) * N + j + 1).y;
	//difference
	float d1 = h - h1;
	float d2 = h - h2;
	float d3 = h - h3;
	float d4 = h - h4;

	const float T = 4.f / float(N);
	float dTotal = 0.f;
	float dMax = glm::max(d1, glm::max(d2, glm::max(d3, d4)));
	if (d1 > T) dTotal += d1;
	if (d2 > T) dTotal += d2;
	if (d3 > T) dTotal += d3; 
	if (d4 > T) dTotal += d4;
	//deposit soil to lower neighbors according to ratio d/dTotal
	v->at((i - 1) * N + j - 1).y = getNewHeight(h1, d1, dMax, dTotal, T);
	v->at((i - 1) * N + j + 1).y = getNewHeight(h2, d2, dMax, dTotal, T);
	v->at((i + 1) * N + j - 1).y = getNewHeight(h3, d3, dMax, dTotal, T);
	v->at((i + 1) * N + j + 1).y = getNewHeight(h4, d4, dMax, dTotal, T);
	v->at((i)*N + j).y -= v->at((i - 1) * N + j - 1).y + v->at((i - 1) * N + j + 1).y +
		v->at((i + 1) * N + j - 1).y + v->at((i + 1) * N + j + 1).y - h1 - h2 - h3 - h4;

}
void ThermalErosion(vector<vec3> *v, int N)
{
#pragma omp parallel for
	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < N - 1; j++) {
			iteration(v, i, j, N);
		}
	}

}
