#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Path2d.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/params/Params.h"
#include "Tree.h"

using namespace ci;
using namespace ci::app;
using namespace std;


//for cairo to link properly
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

class SpaceColonizationApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;

private:
	std::unique_ptr<Tree> tree_m;
	params::InterfaceGlRef params_m;
	void start();

};

void SpaceColonizationApp::setup()
{
	getWindow()->setTitle("Space Colonization");
	screenWidth_g = 800;
	screenHeight_g = 600;
	setWindowSize(screenWidth_g, screenHeight_g);

	params_m = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(170, 130)));
	maxDistance_g = 80.0f;
	minDistance_g = 10.0f;
	params_m->addParam("Leaves", &leavesNo_g, "min=100.0 max=1000.0 step=10.0");
	params_m->addParam("Max Dist", &maxDistance_g, "min=50.0 max=150.0 step=1.0");
	params_m->addParam("Min Dist", &minDistance_g, "min=5.0 max=50.0 step=1.0");

	params_m->addParam("X Spread", &xFactor_g, "min=20.0 max=500.0 step=10.0");
	params_m->addParam("Y Spread", &yFactor_g, "min=20.0 max=500.0 step=10.0");

	params_m->addButton("Start", std::bind(&SpaceColonizationApp::start, this));


	ci::Rand::randSeed(uint32_t(time(NULL)));

}

void SpaceColonizationApp::start()
{
	if (tree_m)
		tree_m.reset();
	tree_m = std::make_unique<Tree>();
}

void SpaceColonizationApp::update()
{
	if(tree_m)
		tree_m->grow();
}

void SpaceColonizationApp::draw()
{
	
	cairo::SurfaceImage cairoSurface(screenWidth_g, screenHeight_g, true);
	cairo::Context ctx(cairoSurface);
	std::vector<double> dashPatter = { 5,10.0,2 };
	cairo::GradientRadial radialGrad(getWindowCenter(), 0.0f, getWindowCenter(), getWindowWidth());
	radialGrad.addColorStop(0, Color(1, 1, 1));
	radialGrad.addColorStop(1, Color(0.6, 0.6, 0.6));
	ctx.setSource(radialGrad);
	ctx.paint();

	if(tree_m)
		tree_m->show(ctx);
	gl::Texture2dRef texture = gl::Texture::create(cairoSurface.getSurface());
	gl::draw(texture);
	params_m->draw();
}

CINDER_APP( SpaceColonizationApp, RendererGl)
