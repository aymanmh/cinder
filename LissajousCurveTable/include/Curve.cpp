#include "Curve.h"


Curve::Curve()
{
}

Curve::~Curve()
{
}

void Curve::setX(float x)
{
	last_m.x = current_m.x;
	current_m.x = x;
}

void Curve::setY(float y)
{
	last_m.y = current_m.y;
	current_m.y = y;
}

void Curve::addPoint()
{
	points_m.push_back(current_m);				
}

ci::vec2 Curve::getCurrent()
{
	return current_m;
}

void Curve::draw(ci::cairo::Context &ctx)
{
	//for (int i = 1 ; i < points_m.size();i++)
	//{
	//	ctx.newSubPath();
	//	ctx.setSource(ci::Color(0,0,0));
	//	ctx.line(points_m[i - 1], points_m[i]);
	//	ctx.stroke();
	//}	

	if (last_m != current_m && last_m != ci::vec2(0,0))
	{
		ctx.newSubPath();
		ctx.setSource(ci::Color(0, 0, 0));
		ctx.line(last_m, current_m);
		ctx.stroke();
	}
	/*ctx.newSubPath();
	ctx.setSource(ci::Color(1, 1, 1));
	ctx.circle(current_m.x, current_m.y, 4);
	ctx.fill();*/
}

void Curve::reset()
{
	points_m.clear();
}

int Curve::pairFunction(float x, float y)
{
	//F(x,y)=.5(x+y)(x+y+1)+y.
	return 0.5 * ( x + y) * (x + y + 1) + y;
}
