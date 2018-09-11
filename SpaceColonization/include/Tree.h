#pragma once
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "cinder/Path2d.h"
#include "cinder/cairo/Cairo.h"

#include "Branch.h"
#include "Leaf.h"
extern int screenWidth_g;
extern int screenHeight_g;
extern float maxDistance_g;
extern float minDistance_g;
extern int leavesNo_g;
//to control std deviation of the gaussian distribution
extern float xFactor_g;
extern float yFactor_g;

class Tree
{
public:
	Tree();
	~Tree();
	void show(ci::cairo::Context &ctx);
	void grow();

private:
	std::vector<std::unique_ptr<Leaf>> leaves_m;
	std::vector<std::shared_ptr<Branch>> branches_m;
};
