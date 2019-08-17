#pragma once
#include <vector>
#include "cinder/gl/gl.h"
#include "cinder/cairo/Cairo.h"

using namespace std;
class Curve
{
public:
	Curve();
	~Curve();
	void setX(float x);
	void setY(float y);
	void addPoint();
	ci::vec2 getCurrent();
	void draw(ci::cairo::Context &ctx);
	void reset();
private:
	vector<ci::vec2> points_m;
	set<int> allocatedIndex_m;
	ci::vec2 current_m;
	ci::vec2 last_m;

	int pairFunction(float x, float y);

};
