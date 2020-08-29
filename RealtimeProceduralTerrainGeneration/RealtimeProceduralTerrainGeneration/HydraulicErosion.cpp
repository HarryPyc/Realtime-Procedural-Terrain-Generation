#include "HydraulicErosion.h"
#include <ctime>
#include <iostream>

struct Particle {
	Particle(vec2 _pos) { 
		pos = _pos;
	}

	vec2 pos;
	vec2 speed = vec2(0); 

	float volume = 1.0f;   
	float sediment = 0.0f; 
};
vec3 GetNormal(vector<vec3>* v, int i, int j, int N) {
	vec3 left = j == 0 ? v->at(i * N + j) : v->at(i * N + j - 1);
	vec3 right = j == N - 1 ? v->at(i * N + j) : v->at(i * N + j + 1);
	vec3 up = i == 0 ? v->at(i * N + j) : v->at((i - 1) * N + j);
	vec3 down = i == N - 1 ? v->at(i * N + j) : v->at((i + 1) * N + j);
	return normalize(cross(right - left, up - down));
}
void HydraulicErosion(vector<vec3>* v, int N, int times)
{
	time_t start, finish;
	time(&start);
	#pragma omp parallel for
	for (int i = 0; i < times*1000; i++) {
		Particle *drop = new Particle(vec2(rand() % N, rand() % N));

		while (drop->volume > minVol) {

			ivec2 ipos = drop->pos;
			vec3 n = GetNormal(v , ipos.x, ipos.y, N);  

			//simulate the movement of droplets
			drop->speed += dt * vec2(n.z, n.x) / (drop->volume * density);
			drop->pos += dt * drop->speed;
			drop->speed *= (1.0f - dt * friction);      
			//eliminate particles that are out of the map
			if (!all(greaterThanEqual(drop->pos, vec2(0))) || !all(lessThan(drop->pos, vec2(N,N)))) 
				break;

			ivec2 _ipos = drop->pos;
			//equilibrium concentration 
			float maxSediment = drop->volume * length(drop->speed) * 
				(v->at(ipos.x * N+ipos.y).y - v->at(_ipos.x * N + _ipos.y).y);
			if (maxSediment < 0.0f) 
				maxSediment = 0.0f;
			//dissolving and deposition
			float diff = maxSediment - drop->sediment;
			drop->sediment += dt * Ks * diff;
			v->at(ipos.x * N + ipos.y).y -= dt * drop->volume * Ks * diff;
			//evaporation
			drop->volume *= (1.0 - dt * Ke);
		}
		delete drop;
	}
	time(&finish);
	cout << "Time Used : " << difftime(finish,start) << " seconds" << endl;
	
}
