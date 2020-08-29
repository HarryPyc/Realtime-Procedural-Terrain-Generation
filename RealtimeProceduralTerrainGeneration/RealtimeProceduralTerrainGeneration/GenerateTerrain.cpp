#include "GenerateTerrain.h"


GLuint create_terrain_vbo(vector<vec3> *vert,int N) {
	vector<vec3> v;
	vector<vec3> normal;
	vector<vec3> texcoord;
	for (int i = 1; i < N-1; i++) {
		for (int j = 1; j < N-1; j++) {
			vec3 left, right, up, down;
			up = vert->at((i - 1) * N + j);
			down = vert->at((i + 1) * N + j);
			left = vert->at(i * N + j - 1);
			right = vert->at(i * N + j + 1);
			vec3 p = vert->at(i * N + j);
			v.push_back(p);
			vec3 uv((p.x + width / 2.f) / width, (p.z + width / 2.f) / width, 0.0f);
			
			texcoord.push_back(uv);
			vec3 n = normalize(cross(right - left, up - down));
			normal.push_back(n);

		}
	}
	v.insert(v.end(), normal.begin(), normal.end());
	v.insert(v.end(), texcoord.begin(), texcoord.end());
	GLuint vbo = -1;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec3), &v[0], GL_STATIC_DRAW);
	
	return vbo;
}
GLuint create_terrain_ebo(int N) {
	vector<unsigned int> indices;
	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < N; j++) {
			indices.push_back(j + i * N);
			indices.push_back(j + (i + 1)*N);
		}
		indices.push_back(RESTART);
	}
	GLuint ebo = -1;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glPrimitiveRestartIndex(RESTART);
	return ebo;
}
GLuint create_terrain_vao(vector<vec3> *v,int N) {
	GLuint vao = -1;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = create_terrain_vbo(v,N);
	N = N - 2;
	GLuint ebo = create_terrain_ebo(N);

	const GLuint pos_loc = 0;
	const GLuint normal_loc = 1;
	const GLuint texcoord_loc = 2;
	glEnableVertexAttribArray(pos_loc);
	glEnableVertexAttribArray(normal_loc);
	glEnableVertexAttribArray(texcoord_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, false, 0, (void*)(N*N*3*sizeof(float)));
	glVertexAttribPointer(texcoord_loc, 3, GL_FLOAT, false, 0, (void*)(N*N * 3 * 2 * sizeof(float)));
	glBindVertexArray(0);
	return vao;
}
void DrawTerrain(GLuint vao, int N) {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLE_STRIP, 2*N*N + N-1, GL_UNSIGNED_INT, 0);
}


bool comp(const vec3 &a, const vec3 &b) {
	float A = a.x + a.z*10000.f;
	float B = b.x + b.z*10000.f;
	return A - B < FLT_EPSILON;
}

Terrain::Terrain(float ratio, bool bThermal, int tTime, bool bHydraulic, int hTime)
{
	init();
	update(ratio, bThermal, tTime, bHydraulic, hTime);
}
Terrain::~Terrain()
{
	delete surf, vor;
}
void Terrain::init() {
	//get Voronoi map and noise map
	vor = new Voronoi(iterationTimes, randomPointsNum, -1.0f, 1.0f, width);
	surf = new Surf(width, initialSpread);
	surf = MidpointDisplacement(surf, iterationTimes);
	N = vor->N;
	//The map we get from midpoint displacement is disordered
	for (int i = 0; i < N * N; i++)
		surf->v.push_back(surf->points[i]->p);
	sort(surf->v.begin(), surf->v.end(), comp);

}
void Terrain::update(float ratio, bool bThermal, int tTime, bool bHydraulic, int hTime) {
	v.clear();
	//initHydraulicErosion(N);
	Tcount = 0;
	Hcount = 0;
	//Addition of noise and Voronoi map
	for (int i = 0; i < N * N; i++) {
		v.push_back(surf->v[i]);
		v[i].y = surf->v[i].y * ratio + vor->v[i].y * (1.f - ratio);
		hMin = v[i].y < hMin ? v[i].y : hMin;
		hMax = v[i].y > hMax ? v[i].y : hMax;
	}
	//if(enableTurb)
	//	turb(c);
	if (bHydraulic)
		hydraulic(hTime);
	if (bThermal) {
		thermal(tTime);
	}

}

void Terrain::thermal(int time) {
	for (int i = 0; i < time; i++) {
		ThermalErosion(&v, N);
	}
	Tcount += time;
}
void Terrain::hydraulic(int time) {
	HydraulicErosion(&v, N, time);
	Hcount += time;
}
void Terrain::turb(float c) {
	vector<vec3>::iterator it = v.begin();
	for (it; it < v.end(); it++) {
		it->y = (1.f+sin((it->x+c*it->y)))/2.f ;
	}
}