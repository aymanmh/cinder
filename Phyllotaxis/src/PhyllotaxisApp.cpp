#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PhyllotaxisApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
	gl::FboRef mFbo;
	int windowWidth_m = 1024;
	int windowHeight_m = 768;
	float n_m = 0.0f;
	float nIncrement_m = 1.0f;
	float angle_m = 137.5f;
	int scale_m = 8;
	params::InterfaceGlRef params_m;
	bool pause = true;
private:
	void reset();
	void startStop();
};

void PhyllotaxisApp::setup()
{
	getWindow()->setTitle("Phyllotaxis");
	setWindowSize(windowWidth_m, windowHeight_m);
	setFrameRate(50.0f);

	params_m = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(200, 120)));
	params_m->addParam("Angle", &angle_m, "min=1.0 max=179.0 step=0.01");
	params_m->addParam("Scaling factor", &scale_m, "min=0.0 max=50.0 step=1.0");
	params_m->addParam("Inc N by", &nIncrement_m, "min=0.1 max=3.0 step=0.1");
	params_m->addButton("Start/Stop", std::bind(&PhyllotaxisApp::startStop, this));	

	params_m->addButton("Reset", std::bind(&PhyllotaxisApp::reset, this));

	mFbo = gl::Fbo::create(windowWidth_m, windowHeight_m);

	gl::clear(Color(0, 0, 0));
}

void PhyllotaxisApp::mouseDown( MouseEvent event )
{
}

void PhyllotaxisApp::startStop()
{
	if (pause)
		pause = false;
	else
		pause = true;

}

void PhyllotaxisApp::reset()
{
	n_m = 0;
	gl::ScopedFramebuffer fbScp(mFbo);
	gl::clear(Color(0, 0, 0));
}

void PhyllotaxisApp::update()
{
	if (pause) return;
	gl::ScopedFramebuffer fbScp(mFbo);

	/*
	Vogel's formula: a=n*137.5 , r=c√n
where:
 n is the ordering number of a floret, counting outward from thecenter. This is the reverse of floret age in a real plant.
 a is the angle between a reference direction and the position vec-tor of thenthfloret in a polar coordinate system originating atthe center of the capitulum.  It follows that thedivergence an-glebetween the position vectors of any two successive florets isconstant,α= 137.5◦.
 r is the distance between the center of the capitulum and thecenter of thenthfloret, given a constant scaling parameterc.
	*/
	float a = glm::radians(n_m * angle_m);
	float r = scale_m * sqrtf(n_m);

	float x = r * sinf(a) + windowWidth_m / 2.f;
	float y = r * cosf(a) + windowHeight_m / 2.f;

	//gl::color( Color(CM_HSV, lmap<float>( (int)a%256,0,255, 0.f, 1.f),1.f,1.f));
	gl::color( Color(CM_HSV, lmap<float>( (int)n_m%256,0,255, 0.f, 1.f),1.f,1.f));
	//gl::color( Color8u((int)n_m%256,(int)r%256, (int)r % 256));
	gl::drawSolidCircle(vec2(x, y), 4);
	n_m+= nIncrement_m;
	gl::color(Color::white());
}

void PhyllotaxisApp::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::draw(mFbo->getColorTexture());
	params_m->draw();
}

CINDER_APP( PhyllotaxisApp, RendererGl )
