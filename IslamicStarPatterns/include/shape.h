#pragma once
#include <vector>
#include "cinder/cairo/Cairo.h"
#include "cinder/gl/gl.h"
using namespace ci;

extern float angle_g;
extern float sides_g;
extern float delta_g;
extern bool showGrid_g;

class Edge
{
public:
	Edge(vec2 a, vec2 b, ColorA color = ColorA(0.1f, 0.1f, 0.1f,1.f));
	~Edge();
	void show(ci::cairo::Context &ctx);
	static ColorA gridColor_s;
	void hankin();
private:
	vec2 v1_m, v2_m, offset1_m,offset2_m;
	vec2 a_m, b_m;
	ColorA edgeColor_m;
};

class Polygon
{
public:
	Polygon(ColorA color);
	~Polygon();
	void show(ci::cairo::Context &ctx);
	void addVertex(int x, int y);
	void close();
	void hankin();

private:
	ColorA color_m;
	std::vector<std::unique_ptr<Edge>> edges_m;
	std::vector<vec2> vertices_m;
};