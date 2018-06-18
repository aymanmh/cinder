#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class DoublePendulumApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
	int windowWidth_m = 900;
	int windowHeight_m = 600;
	void reset();

	float r1;
	float r2;
	float m1;
	float m2;
	float a1;
	float a2;
	float a1_v ;
	float a2_v ;
	float g;

	float uir1 = 150;
	float uir2 = 200;
	float uim1 = 20;
	float uim2 = 20;
	float uia1 = 30.0f;
	float uia2 = 20.0f;	
	float uig = 1.0f;

	gl::Texture2dRef canvasTexture_m;
	Surface8uRef canvasSurface_m;
	params::InterfaceGlRef params_m;
};

void DoublePendulumApp::setup()
{
	getWindow()->setTitle("Double Pendulum");
	setWindowSize(windowWidth_m, windowHeight_m);
	setFrameRate(30.0f);
	
	Surface mySurface(windowWidth_m, windowHeight_m, false, SurfaceChannelOrder::RGB);
	canvasSurface_m = std::make_shared<Surface8u>(mySurface);

	params_m = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(200, 180)));

	params_m->addParam("Arm 1 Length", &uir1, "min=2.0 max=500.0 step=1.0");
	params_m->addParam("Arm 2 Length", &uir2, "min=-2.0 max=500.0 step=1.0");
	params_m->addParam("Angle 1", &uia1, "min= 0.0 max= 90.0 step= 1.0");
	params_m->addParam("Angle 2", &uia2, "min= 0.0 max= 90.0 step= 1.0");
	params_m->addParam("Mass 1", &uim1, "min= 5.0 max= 400.0 step= 5.0");
	params_m->addParam("Mass 2", &uim2, "min= 5.0 max= 400.0 step= 5.0");
	params_m->addParam("Gravity", &uig, "min= 0.0 max= 10.0 step= 0.1");
	params_m->addButton("Apply", std::bind(&DoublePendulumApp::reset, this));

	reset();
}

void DoublePendulumApp::reset()
{
	Area area(0, 0, canvasSurface_m->getWidth(), canvasSurface_m->getHeight());
	Surface::Iter newIter(canvasSurface_m->getIter(area));
	while (newIter.line())
	{
		while (newIter.pixel())
		{
			newIter.r() = newIter.g() = newIter.b() = 0;
		}
	}

	r1 = uir1;
	r2 = uir2;
	m1 = uim1;
	m2 = uim2;
	a1 = glm::radians(uia1);
	a2 = glm::radians(uia2);
	g = uig;
	a1_v = 0.0f;
	a2_v = 0.0f;
}

void DoublePendulumApp::mouseDown( MouseEvent event )
{
}

void DoublePendulumApp::update()
{
	canvasTexture_m = gl::Texture2d::create(*canvasSurface_m);
}

void DoublePendulumApp::draw()
{

	float num1 = -g * (2.0f * m1 + m2) * sin(a1);
	float num2 = -m2 * g * sin(a1 - 2.0f * a2);
	float num3 = -2.0f * sin(a1 - a2)*m2;
	float num4 = a2_v * a2_v*r2 + a1_v * a1_v*r1*cos(a1 - a2);
	float den = r1 * (2.0f * m1 + m2 - m2 * cos(2.0f * a1 - 2.0f * a2));
	float a1_a = (num1 + num2 + num3 * num4) / den;

	num1 = 2.0f * sin(a1 - a2);
	num2 = (a1_v*a1_v*r1*(m1 + m2));
	num3 = g * (m1 + m2) * cos(a1);
	num4 = a2_v * a2_v*r2*m2*cos(a1 - a2);
	den = r2 * (2.0f * m1 + m2 - m2 * cos(2.0f * a1 - 2.0f * a2));
	float a2_a = (num1*(num2 + num3 + num4)) / den;
	gl::clear( Color( 0, 0, 0 ) );
	gl::draw(canvasTexture_m, vec2(0.0f, 0.0f));

	gl::pushModelMatrix();	
	gl::translate(windowWidth_m/2, 50);


	float x1 = r1 * sin(a1);
	float y1 = r1 * cos(a1);

	float x2 = x1 + r2 * sin(a2);
	float y2 = y1 + r2 * cos(a2);


	gl::color(Color::white());
	gl::drawLine(vec2(0.0f, 0.0f), vec2(x1, y1));	
	gl::drawSolidCircle(vec2(x1, y1), m1, m1);
	
	gl::drawLine(vec2(x1, y1), vec2(x2, y2));	
	gl::drawSolidCircle(vec2(x2, y2), m2, m2);
	

	gl::popModelMatrix();

	canvasSurface_m->setPixel(vec2(x2+ windowWidth_m / 2, y2+50.0f), Color8u(randInt(255), randInt(255), randInt(255)));
	
	a1_v += a1_a;
	a2_v += a2_a;
	a1 += a1_v;
	a2 += a2_v;

	params_m->draw();
}

CINDER_APP( DoublePendulumApp, RendererGl )
