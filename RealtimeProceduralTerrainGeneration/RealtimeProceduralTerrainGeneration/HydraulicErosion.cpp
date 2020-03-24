#include "HydraulicErosion.h"

void WaterDistribution(vector<vec3>* v, vector<float>* w, vector<float>* m, int i, int j, int N);
void HydraulicErosion(vector<vec3>* v, int N, int times)
{
	vector<float>* w = new vector<float>(N * N, 0.f);
	vector<float>* m = new vector<float>(N * N, 0.f);
	
	for (int c = 0; c < times; c++) {
		//step 1: stimulate rains
		vector<float>::iterator itw = w->begin();
		for (itw; itw < w->end(); itw++) {
			*itw += Kr;
		}
		//step 2: dissovle
		for (int i = 0; i < N * N; i++) {
			float deltaM = w->at(i) * Ks;
			v->at(i).y -= deltaM;
			m->at(i) += deltaM;
		}
		//step 3: water transportation
		for (int i = 1; i < N - 1; i++) {
			for (int j = 1; j < N - 1; j++) {
				WaterDistribution(v, w, m, i, j, N);
			}
		}
		//step 4: water evaporation
		for (int i = 0; i < N * N; i++) {
			w->at(i) *= (1-Ke);
			float mMax = Kc * w->at(i);
			float deltaM = glm::max(0.f, m->at(i) - mMax);
			m->at(i) -= deltaM;
			v->at(i).y += deltaM;
		}
	}

}
void WaterDistribution(vector<vec3>* v, vector<float>* w, vector<float>* m, int i, int j, int N) {
	vector<float> a, d;
	float water = w->at(i * N + j);
	float mat = m->at(i * N + j);
	float A = v->at(i * N + j).y + w->at(i * N + j);
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
	a_bar /= 9.f;
	float deltaA = A - a_bar;
	if (deltaA < 0) return;
	float deltaW = glm::min(water, deltaA);
	int indexD = 0;
	for (int _i = i - 1; _i <= i + 1; _i++) {
		for (int _j = j - 1; _j <= j + 1; _j++) {
			if (d[indexD] > 0) {
				w->at(_i * N + _j) += deltaW * d[indexD] / dTotal;
				m->at(_i * N + _j) += deltaW / water * mat;
			}
			indexD++;
		}
	}

}
