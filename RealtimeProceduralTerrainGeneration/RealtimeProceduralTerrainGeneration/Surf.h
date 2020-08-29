#pragma once
#include "Mathfunc.h"
#include <omp.h>
class Square;
class Edge;
class Point;

class Point {
public:
	vec3 p;
	vec3 n;
	vec3 left, right, up, down;//4 neighbors

	Point(vec3 pos);
	void GetNormal();
};
class Edge {
public:
	Point *a, *b;
	vector<Square*> squares;//neighbor faces
	Point *mid;
	Edge *ne0, *ne1;//new edges after displacement
	Edge(Point *a, Point *b);
	~Edge();
	void GetMidPoint(float spread);
	void GenerateNewEdges();
};

class Square {
public:
	Edge *e0, *e1, *e2, *e3;
	Point *mid;
	//new edges and faces after displacement
	Edge *ne0, *ne1, *ne2, *ne3;
	Square *nf0, *nf1, *nf2, *nf3;
	Square(Edge *e0, Edge *e1, Edge *e2, Edge *e3);
	~Square();
	void GetMidPoint(float spread);
	void GenerateNewSquares();
};

class Surf
{
public:
	vector<Point*> points;
	vector<Edge*> edges;
	vector<Square*> squares;
	vector<vec3> v;//all vetices

	Surf(float w, float spread);
	Surf();
	~Surf();

	float spread;
	void init(float w, float spread);

	void CalculateNormal();
};

Surf* MidpointDisplacement(Surf* origin, int times);
inline vec3 offset(float spread);
