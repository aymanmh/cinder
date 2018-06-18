#include "Branch.h"

float Branch::rigtAngle_s;
float Branch::leftAngle_s;
float Branch::shrinkFactor_s;
Color Branch::color_s;

Branch::Branch(vec2 start, vec2 end):start_m(start),end_m(end)
{
}

void Branch::show()
{
	gl::color(color_s);
	glLineWidth(2);
	gl::drawLine(start_m, end_m);
}

std::pair<Branch,Branch> Branch::grow()
{	
	vec2 direction = end_m - start_m;
	vec2 rightDir = glm::rotate(direction, glm::radians(Branch::rigtAngle_s));
	vec2 leftDir = glm::rotate(direction, glm::radians(Branch::leftAngle_s));
	
	rightDir *= Branch::shrinkFactor_s;
	leftDir *= Branch::shrinkFactor_s;
	vec2 newEndRight = end_m + rightDir;
	vec2 newEndLeft = end_m + leftDir;

	Branch right = Branch(end_m, newEndRight);
	Branch left = Branch(end_m, newEndLeft);
		
	finished= true;
	return make_pair(left, right);
}

bool Branch::hasFinished() const
{
	return finished;
}