#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/params/Params.h"
#include "universe.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GameOfLifeApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
	void start();
	void stop();
	void random();
	void clear();
	int columns_m;
	int rows_m;
	int resolution_m = 12;
	float windowWidth_m = 600.0f;
	float windowHeight_m = 600.0f;
	params::InterfaceGlRef params_m;
	bool keepGoing_m = false;
	float fps_m = 20.0;
	std::unique_ptr<Universe> universe_m;
};

void GameOfLifeApp::setup()
{
	getWindow()->setTitle("Game of Life");
	setWindowSize(windowWidth_m, windowHeight_m);
	setFrameRate(fps_m);
	Rand::randSeed(uint32_t(time(NULL)));
	
	params_m = params::InterfaceGl::create(getWindow(), "Parameters", toPixels(ivec2(170, 120)));

	params_m->addParam("Speed", &fps_m, "min=1.0 max=60.0 step=1.0");
	params_m->addButton("Start", std::bind(&GameOfLifeApp::start, this));
	params_m->addButton("Stop", std::bind(&GameOfLifeApp::stop, this));
	params_m->addButton("Random", std::bind(&GameOfLifeApp::random, this));
	params_m->addButton("Clear", std::bind(&GameOfLifeApp::clear, this));

	columns_m = windowWidth_m / resolution_m;
	rows_m = windowHeight_m / resolution_m;
	
	universe_m = make_unique<Universe>(rows_m, columns_m);
}

void GameOfLifeApp::mouseDown( MouseEvent event )
{
	if (event.isLeft())
	{
		keepGoing_m = false;
		int x = event.getX() / resolution_m;
		int y = event.getY() / resolution_m;
		universe_m->flipCellState(x, y);
	}
	else if (event.isRight())
		start();
	else
		clear();
}

void GameOfLifeApp::update()
{
	if (keepGoing_m)
	{
		universe_m->tick();
	}
}

void GameOfLifeApp::draw()
{
	//gl::clear( Color8u(192, 192, 192) ); 	
	gl::clear( Color8u(47, 79, 79) ); 	
	auto gird = universe_m->getGrid();
	for (int i = 0; i < columns_m; i++)
	{
		for (int j = 0; j < rows_m; j++)
		{
			int x = i * resolution_m;
			int y = j * resolution_m;
			if (gird[i+j*columns_m] == 1)
			{				
				gl::color(0, 0, 0);
				gl::drawSolidRect(Rectf(x, y, x+resolution_m-1 , y + resolution_m - 1));
			}
		}
	}

	params_m->draw();
}

void GameOfLifeApp::start()
{
	setFrameRate(fps_m);
	keepGoing_m = true;
}

void GameOfLifeApp::stop()
{
	keepGoing_m = false;
}

void GameOfLifeApp::random()
{
	keepGoing_m = false;
	universe_m->randomize();
}

void GameOfLifeApp::clear()
{
	keepGoing_m = false;
	universe_m->clear();
}

CINDER_APP( GameOfLifeApp, RendererGl )
