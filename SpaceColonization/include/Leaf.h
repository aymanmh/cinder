#pragma once
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "cinder/Path2d.h"
#include "cinder/cairo/Cairo.h"

extern int screenWidth_g;
extern int screenHeight_g;
extern float maxDistance_g;
extern float minDistance_g;
extern int leavesNo_g;
//to control std deviation of the gaussian distribution
extern float xFactor_g;
extern float yFactor_g;

class Leaf
{
public:
	Leaf();
	~Leaf();
	void show(ci::cairo::Context &ctx);
	ci::vec2 getPosition();
	bool reached_m = false;

private:
	ci::vec2 position_m;
	static ci::Color color_s;

};