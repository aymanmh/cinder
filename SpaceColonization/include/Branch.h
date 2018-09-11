#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Path2d.h"
#include "cinder/cairo/Cairo.h"

extern float length_g;

class Branch : public std::enable_shared_from_this<Branch>
{
public:
	Branch(ci::vec2 position, ci::vec2 direction,std::shared_ptr<Branch> parent);
	~Branch();
	ci::vec2 getPosition();
	ci::vec2 getDirection();
	void setNextDirection(ci::vec2 newDirection);
	std::shared_ptr<Branch> grow();
	void show(ci::cairo::Context &ctx);
	int nearbyLeavesCount_m = 0;

private:
	ci::vec2 position_m;
	ci::vec2 direction_m;
	ci::vec2 nextDirection_m;
	std::shared_ptr<Branch> parent_m;
	static ci::Color color_s;
};