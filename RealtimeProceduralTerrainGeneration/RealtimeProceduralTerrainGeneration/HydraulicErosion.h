#pragma once
#include "Mathfunc.h"
#include <omp.h>

const float Ke = 0.001f;//evaporation rate
const float Ks = 0.1f;//dissolving rate
const float Kc = 0.01f;
const float dt = 1.2f; //time-step to control whole process
const float density = 1.0f;  //increase this will make particle move faster

const float minVol = 0.01f;
const float friction = 0.05f;

void HydraulicErosion(vector<vec3>* v, int N, int times);
