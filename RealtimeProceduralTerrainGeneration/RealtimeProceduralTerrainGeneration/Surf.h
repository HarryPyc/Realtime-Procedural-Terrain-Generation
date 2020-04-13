#pragma once
#include "Mathfunc.h"

class Square;
class Edge;
class Point;

class Point {
public:
	vec3 p;
	vec3 n;
	vec3 left, right, up, down;

	Point(vec3 pos);
	void GetNormal();
};
class Edge {
public:
	Point *a, *b;
	vector<Square*> squares;
	Point *mid;
	Edge *ne0, *ne1;
	Edge(Point *a, Point *b);
	void GetMidPoint(float spread);
	void GenerateNewEdges();
};

class Square {
public:
	Edge *e0, *e1, *e2, *e3;
	Point *mid;

	Edge *ne0, *ne1, *ne2, *ne3;
	Square *nf0, *nf1, *nf2, *nf3;
	Square(Edge *e0, Edge *e1, Edge *e2, Edge *e3);
	void GetMidPoint(float spread);
	void GenerateNewSquares();
};

class Surf
{
public:
	vector<Point*> points;
	vector<Edge*> edges;
	vector<Square*> squares;
	vector<vec3> v;

	Surf(float w, float spread);
	Surf();
	~Surf();

	float spread;
	Surf* MidpointDisplacement();
	void CalculateNormal();
};


inline vec3 offset(float spread);
