#pragma once
#include "cinder/gl/gl.h"

using namespace ci;
using namespace std;

class Branch
{
public:
	Branch(vec2 start, vec2 end);
	void show();
	std::pair<Branch, Branch> grow();
	bool hasFinished() const;

public:
	static float rigtAngle_s;
	static float leftAngle_s;
	static float shrinkFactor_s;
	static Color color_s;

private:
	vec2 start_m;
	vec2 end_m;
	bool finished = false;
};