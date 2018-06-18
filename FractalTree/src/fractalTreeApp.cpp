#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"
#include "cinder/params/Params.h"

#include "Branch.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class FractalTreeApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
	void createTrunk();

	float windowWidth_m = 800.0f;
	float windowHeight_m = 600.0f;	
	float rightAngle_m = 37.0f;
	float leftAngle_m = -37.0f;
	float shrinkFactor_m = 0.67f;
	float trunkLength_m = 150.0f;
	params::InterfaceGlRef params_m;
	std::vector<shared_ptr<Branch>> tree_m;
};

void FractalTreeApp::setup()
{
	getWindow()->setTitle("Fractal Tree");
	setWindowSize(windowWidth_m, windowHeight_m);
	setFrameRate(10.0f);
	
	params_m = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(200, 120)));

	params_m->addParam("Right Angle", &rightAngle_m, "min=1.0 max=179.0 step=1.0");
	params_m->addParam("Left Angle", &leftAngle_m, "min=-179.0 max=-1.0 step=1.0");
	params_m->addParam("Shrink Factor", &shrinkFactor_m, "min= 0.1 max= 0.9 step= 0.01");
	params_m->addParam("Trunk Length", &trunkLength_m, "min= 20.0 max= 400.0 step= 5.0");
	params_m->addButton("Reset", std::bind(&FractalTreeApp::createTrunk, this));

	createTrunk();
}

void FractalTreeApp::mouseDown( MouseEvent event )
{
	if (event.isRight())
	{
		createTrunk();
		return;
	}

	int treeSize = tree_m.size() - 1;
	for (int i = treeSize; i >= 0; --i)
	{
		if (! (tree_m[i]->hasFinished()) )
		{
			auto branches = tree_m[i]->grow();
			tree_m.emplace_back(make_shared<Branch>(branches.first));
			tree_m.emplace_back(make_shared<Branch>(branches.second));
		}
	}
}

void FractalTreeApp::createTrunk()
{
	tree_m.clear();
	Branch::color_s = Color(75.0f / 255.0f, 0.0f, 130.0f / 255.0f);
	Branch::leftAngle_s = leftAngle_m;
	Branch::rigtAngle_s = rightAngle_m;
	Branch::shrinkFactor_s = shrinkFactor_m;

	shared_ptr<Branch> trunk = make_shared<Branch>(Branch(vec2(windowWidth_m / 2.0f, windowHeight_m), vec2(windowWidth_m / 2.0f, windowHeight_m - trunkLength_m)));
	tree_m.emplace_back(trunk);
}

void FractalTreeApp::update()
{
}

void FractalTreeApp::draw()
{
	gl::clear( Color( 0.5f, 0.5f, 0.5f ) );
	for (auto& branch : tree_m)
	{
		branch->show();
	}

	params_m->draw();
}

CINDER_APP( FractalTreeApp, RendererGl )
