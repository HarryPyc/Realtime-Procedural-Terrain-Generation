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
int vN;
GLuint create_voronoi_vbo() {
	Voronoi *vor = new Voronoi(iterationTimes, randomPointsNum, -1.0f, 1.0f);
	vN = vor->N;
	GLuint vbo = -1;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vor->v.size() * sizeof(vec3), &vor->v[0], GL_STATIC_DRAW);

	return vbo;
}
GLuint create_voronoi_ebo() {
	vector<unsigned int> indices;
	for (int i = 0; i < vN - 1; i++) {
		for (int j = 0; j < vN; j++) {
			indices.push_back(j + i*vN);
			indices.push_back(j + (i + 1)*vN);
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
void DrawTerrain(GLuint vao) {
	glBindVertexArray(vao);
	//glDrawArrays(GL_TRIANGLES, 0, N);
	
	glDrawElements(GL_TRIANGLE_STRIP, 2*vN*vN + vN-1, GL_UNSIGNED_INT, 0);
}