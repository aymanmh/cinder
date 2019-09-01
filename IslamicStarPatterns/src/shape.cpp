#include "shape.h"

extern float angle_g = 60.f;
extern float sides_g = 4.f;
extern float delta_g = 10.f;
extern bool showGrid_g = false;

ColorA Edge::gridColor_s = ColorA(.9, .9, .9,.2);


Edge::Edge(vec2 a, vec2 b, ColorA color) :a_m(a), b_m(b), edgeColor_m(color)
{
}

Edge::~Edge()
{
}

void Edge::show(ci::cairo::Context &ctx)
{
	if (showGrid_g)
	{
		ctx.setSource(gridColor_s);
		ctx.line(a_m, b_m);
		ctx.stroke();
	}
	ctx.setSource(edgeColor_m);
	ctx.line(offset1_m, offset1_m+v1_m);
	ctx.stroke();
	ctx.setSource(edgeColor_m);
	ctx.line(offset2_m, offset2_m+v2_m);
	ctx.stroke();
}

void Edge::hankin()
{
	auto mid = a_m + b_m;
	mid /= 2;
	v1_m = mid - a_m;
	v2_m = mid - b_m;
	auto edgeLen = glm::length(v1_m) + delta_g;
	offset1_m = mid;
	offset2_m = mid;
	vec2 v1new = v1_m;
	vec2 v2new = v2_m;
	if (delta_g > 0)
	{		//set magnitude
		vec2 v1new = glm::normalize(v1_m) *  delta_g;
		vec2 v2new = glm::normalize(v2_m) *  delta_g;

		offset1_m = mid + v2new;
		offset2_m = mid + v1new;
	}
	v1_m = glm::normalize(glm::rotate(v1new, glm::radians(angle_g)));
	v2_m = glm::normalize(glm::rotate(v2new, glm::radians(-1* angle_g)));

	//Law of Sines
	float interior = (sides_g - 2) *  M_PI / sides_g;
	float alpha = interior * 0.5f;
	float beta = M_PI - glm::radians(angle_g) - alpha;
	float hLen = (edgeLen * sin(alpha)) / sin(beta);

	v1_m *= hLen;
	v2_m *= hLen;
}


// ******** the polygon class

Polygon::Polygon(ColorA color) : color_m (color)
{
}

Polygon::~Polygon()
{
}
void Polygon::show(ci::cairo::Context &ctx)
{
	ctx.newSubPath();
	for (auto& edge : edges_m)
	{
		edge->show(ctx);
	}
	ctx.closePath();
	//ctx.setSource(Color(1,0,0));
	//ctx.fill();

}
void Polygon::addVertex(int x, int y)
{
	vec2 tempNew = vec2(x, y);
	if (!vertices_m.empty())
	{
		vec2& prev = vertices_m[vertices_m.size() - 1];
		edges_m.emplace_back(std::make_unique<Edge>(prev, tempNew, color_m));
	}
	vertices_m.emplace_back(tempNew);
}

void Polygon::close()
{
	edges_m.emplace_back(std::make_unique<Edge>(vertices_m[vertices_m.size() - 1], vertices_m[0], color_m));
}

void Polygon::hankin()
{
	for (auto& edge : edges_m)
	{
		edge->hankin();
	}
}
