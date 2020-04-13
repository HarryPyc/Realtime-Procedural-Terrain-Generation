#include "GenerateTerrain.h"

float hMax = -999.f;
float hMin = 999.f;
GLuint create_terrain_vbo(vector<vec3> *vert,int N) {
	vector<vec3> v = *vert;
	vector<vec3> normal;
	vector<vec3> texcoord;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			vec3 left, right, up, down;
			up = i == 0 ? v[j] : v[(i - 1)*N + j];
			down = i == N - 1 ? v[i*N + j] : v[(i + 1)*N + j];
			left = j == 0 ? v[i*N] : v[i*N + j - 1];
			right = j == N - 1 ? v[i*N + j] : v[i*N + j + 1];
			vec3 uv((v[i*N + j].x + width / 2.f) / width, (v[i*N + j].z + width / 2.f) / width, 0.0f);
			
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
	//glDrawArrays(GL_TRIANGLES, 0, N);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLE_STRIP, 2*N*N + N-1, GL_UNSIGNED_INT, 0);
}


bool comp(const vec3 &a, const vec3 &b) {
	float A = a.x + a.z*1000.f;
	float B = b.x + b.z*1000.f;
	return A < B;
}

Terrain::Terrain()
{
	init();
	update();
}
void Terrain::init() {
	
	vor = new Voronoi(iterationTimes, randomPointsNum, -1.0f, 1.0f, width);
	surf = new Surf(width, initialSpread);
	for (int i = 0; i < iterationTimes; i++)
		surf = surf->MidpointDisplacement();
	N = vor->N;
	for (int i = 0; i < N * N; i++)
		surf->v.push_back(surf->points[i]->p);
	sort(surf->v.begin(), surf->v.end(), comp);
	initHydraulicErosion(N);
}
void Terrain::update(float ratio, bool bThermal, int tTime, bool bHydraulic, int hTime) {
	v.clear();
	for (int i = 0; i < N * N; i++) {
		v.push_back(surf->v[i]);
		v[i].y = surf->v[i].y * ratio + vor->v[i].y * (1.f - ratio);
		hMin = v[i].y < hMin ? v[i].y : hMin;
		hMax = v[i].y > hMax ? v[i].y : hMax;
	}
	if (bThermal) {
		thermal(tTime);
	}
	if (bHydraulic)
		hydraulic(hTime);
}

void Terrain::thermal(int time) {
	for (int i = 0; i < time; i++) {
		ThermalErosion(&v, N);
	}
}
void Terrain::hydraulic(int time) {
	HydraulicErosion(&v, N, time);
}