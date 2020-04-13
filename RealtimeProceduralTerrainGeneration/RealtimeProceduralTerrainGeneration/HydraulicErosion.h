#pragma once
#include "Mathfunc.h"
const float Kr = 0.01f;
const float Ks = 0.01f;
const float Ke = 0.5f;
const float Kc = 0.01f;
void HydraulicErosion(vector<vec3>* v, int N, int times);
void initHydraulicErosion(int N);
