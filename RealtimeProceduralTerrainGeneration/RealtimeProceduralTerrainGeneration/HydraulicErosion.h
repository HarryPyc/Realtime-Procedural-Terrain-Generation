#pragma once
#include "Mathfunc.h"
#include <omp.h>
const float Kr = 0.01f;
const float Ks = 0.01f;
const float Ke = 0.5f;
const float Kc = 0.05f;
void HydraulicErosion(vector<vec3>* v, int N, int times);
void initHydraulicErosion(int N);
