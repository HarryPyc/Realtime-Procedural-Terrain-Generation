#include "HydraulicErosion.h"

vector<float> *w, *m;
void initHydraulicErosion(int N) {
	w = new vector<float>(N * N, 0.0f);
	m = new vector<float>(N * N, 0.f);
}
void WaterDistribution(vector<vec3>* v, vector<float>* w, vector<float>* m, int i, int j, int N);
void HydraulicErosion(vector<vec3>* v, int N, int times)
{

	for (int c = 0; c < times; c++) {
		//step 1: stimulate rains
#pragma omp parallel for
		for (int i = 0; i < N * N; i++) {
			w->at(i) += Kr;
		}
		//step 2: dissovle
#pragma omp parallel for
		for (int i = 0; i < N * N; i++) {
			float mMax = w->at(i) * Kc;
			if (m->at(i) < mMax) {
				float deltaM = glm::max(w->at(i) * Ks, mMax - m->at(i));
				v->at(i).y -= deltaM;
				m->at(i) += deltaM;
			}
		}
#pragma omp parallel for
		//step 3: water transportation
		for (int i = 1; i < N - 1; i++) {
			for (int j = 1; j < N - 1; j++) {
				WaterDistribution(v, w, m, i, j, N);
			}
		}
#pragma omp parallel for
		//step 4: water evaporation
		for (int i = 0; i < N * N; i++) {
			if (w->at(i) < 0.005f)
				w->at(i) = 0;
			else
				w->at(i) *= (1-Ke);
			float mMax = Kc * w->at(i);
			float deltaM = glm::max(0.f, m->at(i) - mMax);
			m->at(i) -= deltaM;
			v->at(i).y += deltaM;
		}
	}
	
}
void WaterDistribution(vector<vec3>* v, vector<float>* w, vector<float>* m, int i, int j, int N) {
	vector<float> a, d;//altitude and difference
	//water and material at center
	float water = w->at(i * N + j);
	float mat = m->at(i * N + j);
	//altitude at center
	float A = v->at(i * N + j).y + water;
	float dTotal = 0.f, a_bar = 0.f;
	for (int _i = i - 1; _i <= i + 1; _i++) {
		for (int _j = j - 1; _j <= j + 1; _j++) {
			float h = v->at(_i * N + _j).y + w->at(_i * N + _j);
			a.push_back(h);
			a_bar += h;
			float diff = A - h;
			if (diff > 0) dTotal += diff;
			d.push_back(diff);
		}
	}
	//average altitude
	a_bar /= 9.f;
	float deltaA = A - a_bar;
	if (deltaA <= 0) return;
	float deltaW = glm::min(water, deltaA);
	w->at(i * N + j) -= deltaW;
	int indexD = 0;
	for (int _i = i - 1; _i <= i + 1; _i++) {
		for (int _j = j - 1; _j <= j + 1; _j++) {
			if (d[indexD] > 0) {
				//move water and material to lower neighbors
				w->at(_i * N + _j) += deltaW * d[indexD] / dTotal;
				float deltaM = deltaW /water * mat * d[indexD] / dTotal;
				m->at(_i * N + _j) += deltaM;
				m->at(i * N + j) -= deltaM;
			}
			indexD++;
		}
	}

}
