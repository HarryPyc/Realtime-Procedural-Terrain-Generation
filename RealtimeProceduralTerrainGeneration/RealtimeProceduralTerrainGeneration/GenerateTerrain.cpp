#include "GenerateTerrain.h"

int N;
GLuint create_surf_vbo() {
	vector<vec3> v;
	Surf *surf = new Surf(width, initialSpread);
	for (int i = 0; i < iterationTimes; i++)
		surf = surf->MidpointDisplacement();

	surf->CalculateNormal();

	vector<Square*>::iterator i = surf->squares.begin();
	for (i; i < surf->squares.end(); i++) {
		v.push_back((*i)->e0->a->p);
		v.push_back((*i)->e0->a->n);

		v.push_back((*i)->e0->b->p);
		v.push_back((*i)->e0->b->n);

		v.push_back((*i)->e2->a->p);
		v.push_back((*i)->e2->a->n);

		v.push_back((*i)->e0->b->p);
		v.push_back((*i)->e0->b->n);

		v.push_back((*i)->e2->a->p);
		v.push_back((*i)->e2->a->n);

		v.push_back((*i)->e2->b->p);
		v.push_back((*i)->e2->b->n);
	}
	N = v.size() / 2;
	GLuint vbo = -1;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*v.size(), &v[0], GL_STATIC_DRAW);

	return vbo;
}
GLuint create_surf_vao() {
	GLuint vao = -1;
	GLuint vbo = create_surf_vbo();
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	const GLuint pos_loc = 0;
	const GLuint normal_loc = 1;
	glEnableVertexAttribArray(pos_loc);
	glEnableVertexAttribArray(normal_loc);

	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(0));
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3*sizeof(float)));

	glBindVertexArray(0);
	return vao;
}

GLuint create_voronoi_vbo() {
	Voronoi *vor = new Voronoi(iterationTimes, randomPointsNum, -1.0f, 1.0f, width);
	N = vor->N;
	GLuint vbo = -1;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vor->v.size() * sizeof(vec3), &vor->v[0], GL_STATIC_DRAW);

	return vbo;
}
GLuint create_voronoi_ebo() {
	vector<unsigned int> indices;
	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < N; j++) {
			indices.push_back(j + i*N);
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
GLuint create_voronoi_vao() {
	GLuint vao = -1;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = create_voronoi_vbo();
	GLuint ebo = create_voronoi_ebo();

	const GLuint pos_loc = 0;
	glEnableVertexAttribArray(pos_loc);

	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);

	glBindVertexArray(0);
	return vao;
}

float hMax = -999.f;
float hMin = 999.f;
GLuint create_terrain_vbo() {
	Voronoi *vor = new Voronoi(iterationTimes, randomPointsNum, -1.0f, 1.0f, width);
	Surf *surf = new Surf(width, initialSpread);
	for (int i = 0; i < iterationTimes; i++)
		surf = surf->MidpointDisplacement();
	N = vor->N;
	vector<vec3> v;
	for (int i = 0; i < N*N; i++)
		v.push_back(surf->points[i]->p);
	sort(v.begin(), v.end(), comp);
	for (int i = 0; i < N*N; i++) {
		v[i].y = (v[i].y*2.f + vor->v[i].y*1.f) / 3.f;
		hMin = v[i].y < hMin ? v[i].y : hMin;
		hMax = v[i].y > hMax ? v[i].y : hMax;
	}
	for (int i = 0; i < 50; i++) {
		ThermalErosion(&v, N);
	}

	
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
GLuint create_terrain_ebo() {
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
GLuint create_terrain_vao() {
	GLuint vao = -1;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = create_terrain_vbo();
	GLuint ebo = create_terrain_ebo();

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
void DrawTerrain(GLuint vao) {
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