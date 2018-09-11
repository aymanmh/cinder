#include "Leaf.h"

ci::Color Leaf::color_s = ci::Color(0.3f, 0.7f, 0.25f);

int screenWidth_g;
int screenHeight_g;
float maxDistance_g = 80.0f;
float minDistance_g = 10.0f;
int leavesNo_g = 400;
//to control std deviation of the gaussian distribution
float xFactor_g = 110.f;
float yFactor_g = 80.f;

Leaf::Leaf()
{
	float h = ci::Rand::randGaussian();

	//magic numbers to assure it's mean around the center of the screen
	float x = ci::Rand::randGaussian()*xFactor_g + screenWidth_g / 2; 
	if (x > screenWidth_g)
		x -=  5.0f;
	
	float y = ci::Rand::randGaussian()*yFactor_g + (screenHeight_g - (yFactor_g + 250.f));
	if (y > screenHeight_g)
		y -= 50.0f;
	else if (y < 0)
		y = 5.0f;

	position_m = ci::vec2(x,y);
}

void Leaf::show(ci::cairo::Context &ctx)
{
	ctx.newSubPath();
	ctx.setSource(color_s);
	ctx.circle(position_m.x, position_m.y, 5.0);	
	ctx.fill();
}

ci::vec2 Leaf::getPosition()
{
	return position_m;
}


Leaf::~Leaf()
{
}