#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\norm.hpp>
#include <vector>
#include <algorithm>
using namespace glm;
using namespace std;

inline float random11() {
	float r = 2.f*rand() / (float)RAND_MAX - 1.f;
	return r;
}