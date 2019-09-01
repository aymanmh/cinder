#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/params/Params.h"
#include "cinder/gl/gl.h"
#include "shape.h"

using namespace ci;
using namespace ci::app;
using namespace std;

//for cairo to link properly
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

int screenWidth_g;
int screenHeight_g;
class IslamicStarPatternsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
	void buildHexaCell(int x, int y, int n);
	void buildHexaGrid();
	void builSquaresGrid();
	params::InterfaceGlRef params_m;
	std::vector<std::shared_ptr<Polygon>> myPolygons;
	int inc_m = 1;
	int cellSelection_m = 0;
	vector<string> cellTypes_m;
	unique_ptr<cairo::GradientRadial> radialGrad_m;
	std::chrono::time_point<std::chrono::steady_clock> startTime_m;
};

void IslamicStarPatternsApp::setup()
{
	getWindow()->setTitle("Islamic Star Patterns");
	screenWidth_g = 1280;
	screenHeight_g = 720;
	setWindowSize(screenWidth_g, screenHeight_g);

	setFrameRate(60.0f);
	params_m = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(170, 130)));
	params_m->addParam("Angle", &angle_g, "min=0.0 max=120.0 step=1.0");
	params_m->addParam("Delta", &delta_g, "min=0.0 max=50.0 step=1.0");
	params_m->addParam("Grid", &showGrid_g);

	cellTypes_m = { "Hexagonal","Square" };
	params_m->addParam("Cell Type", cellTypes_m, &cellSelection_m).
		updateFn([this] { if (cellSelection_m == 0) buildHexaGrid(); else builSquaresGrid(); });;
	//angle_g = 0;
	buildHexaGrid();

	radialGrad_m = std::make_unique<cairo::GradientRadial>(getWindowCenter(), 0.0f, getWindowCenter(), getWindowWidth());
	radialGrad_m->addColorStop(0, Color(.1, .1, .1));
	radialGrad_m->addColorStop(1, Color(0.8, 0.8, 0.8));
}

void IslamicStarPatternsApp::mouseDown( MouseEvent event )
{

}

void IslamicStarPatternsApp::update()
{	
	//animation
	//if (angle_g == 90)
	//	inc_m = -1;
	//else if(angle_g == 0)
	//	inc_m = 1;

	//angle_g += inc_m;

}

void IslamicStarPatternsApp::draw()
{	
	cairo::SurfaceImage cairoSurface(screenWidth_g, screenHeight_g, true);
	cairo::Context ctx(cairoSurface);
	ctx.setSource(*radialGrad_m);
	ctx.paint();
	for (auto& poly : myPolygons)
	{
		poly->hankin();
		poly->show(ctx);
	}

	gl::Texture2dRef texture = gl::Texture::create(cairoSurface.getSurface());
	gl::draw(texture);
	params_m->draw();

}

void IslamicStarPatternsApp::buildHexaCell(int x, int y,int n)
{

	int r = 50;

	ColorA edgeColor = ColorA(0, .9f,.2f, 1.f);
	//ColorA edgeColor = ColorA(CM_HSV, lmap<float>((int)n % 256, 0, 255, 0.f, 1.f), 1.f, 1.f, 1.f);
	auto myPoly = std::make_shared<Polygon>(edgeColor);
	auto inc = (2 * M_PI) / sides_g;
	for (int index = 0; index < sides_g; index++) {
		auto phi = (index * inc) - inc / 2;
		auto vX = x + r * cos(phi);
		auto vY = y + r * sin(phi);
		myPoly->addVertex(vX, vY);
	}
	myPoly->close();
	myPolygons.emplace_back(myPoly);
}

void IslamicStarPatternsApp::buildHexaGrid()
{
	myPolygons.clear();
	myPolygons.reserve(100);
	sides_g = 6;
	//return;
	int r = 50;
	int h = r * 2;
	auto w = (sqrt(3) / 2) * h;
	auto inc = 3 * (h / 4);
	int row = 0;
	int n = 0;
	for (int y = -h / 2; y < screenHeight_g + h / 2; y += inc) {
		int startX = ((row % 2) == 0) ? -w : -w / 2;
		for (int x = startX; x < screenWidth_g +inc; x += w) {
			buildHexaCell(x, y, n++);
		}
		row++;
	}
}

void IslamicStarPatternsApp::builSquaresGrid()
{
	myPolygons.clear();
	myPolygons.reserve(100);
	sides_g = 4;
	int inc = 80;
	int n = 0;
	for (size_t x = 0; x < screenWidth_g; x += inc)
	{
		for (size_t y = 0; y < screenHeight_g; y += inc)
		{	
			ColorA edgeColor = ColorA(CM_HSV, lmap<float>((int)n % 256, 0, 255, 0.f, 1.f), 1.f, 1.f,1.f);
			auto myPoly = std::make_shared<Polygon>(edgeColor);
			int inc = 80;
			n++;
			myPoly->addVertex(x, y);
			myPoly->addVertex(x + inc, y);
			myPoly->addVertex(x + inc, y + inc);
			myPoly->addVertex(x, y + inc);
			myPoly->close();
			myPolygons.emplace_back(myPoly);
		}
	}
}

CINDER_APP( IslamicStarPatternsApp, RendererGl )
