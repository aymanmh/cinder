#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Path2d.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/params/Params.h"
#include "Curve.h"

using namespace ci;
using namespace ci::app;
using namespace std;

//for cairo to link properly
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

class LissajousCurneTableApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
	float screenWidth_m = 800;
	float screenHeight_m = 800;
	float w_m = 50.f;
	int cols_m;
	int rows_m;
	float angle_m = 0.f;
	std::vector<std::unique_ptr<Curve>> curves_m;
	cairo::Context ctx2_m;
	cairo::SurfaceImage cairoSurface_m;
	bool start = false;
};

void LissajousCurneTableApp::setup()
{
	getWindow()->setTitle("Lissajous Curve Table");	
	setWindowSize(screenWidth_m, screenHeight_m);
	cols_m = screenWidth_m / w_m - 1;
	rows_m = screenHeight_m / w_m - 1;
	curves_m.resize(rows_m * cols_m);

	cairoSurface_m = cairo::SurfaceImage(screenWidth_m, screenHeight_m, true); 
	
	ctx2_m = cairo::Context(cairoSurface_m);
	ctx2_m.setSource(ColorAf(0.f,0.f,0.f,0.f));
	ctx2_m.paint();

	//cairo::Context ctx_2(cairoSurface);

	for (size_t i = 0; i < curves_m.size(); i++)
		curves_m[i] = make_unique<Curve>();
}

void LissajousCurneTableApp::update()
{
}

void LissajousCurneTableApp::mouseDown(MouseEvent event)
{
	start = true;
}

void LissajousCurneTableApp::draw()
{
	
	//gl::clear( Color( 0, 0, 0 ) ); 
	cairo::SurfaceImage cairoSurface(screenWidth_m, screenHeight_m, true);
	cairo::Context ctx(cairoSurface);
	cairo::GradientRadial radialGrad(getWindowCenter(), 0.0f, getWindowCenter(), getWindowWidth());
	radialGrad.addColorStop(0, Color(1, 1, 1));
	radialGrad.addColorStop(1, Color(0.6, 0.5, 0.6));
	ctx.setSource(radialGrad);
	ctx.paint();
	if (!start)
		return;
	Color clr(0.4f, 0.3f, 0.1f);
	float radius = w_m / 2 - 10;
	for (size_t i = 0; i < cols_m; i++)
	{
		float cx = w_m + i * w_m + w_m / 2;
		float cy = w_m / 2;
		ctx.newSubPath();
		ctx.setSource(clr);
		//ctx.scale(1, 2);
		ctx.circle(cx, cy, radius);// , 0.0, 2 * M_PI);
		ctx.stroke();

		float x = radius * cos(angle_m * (i + 1) - M_PI / 2);
		float y = radius * sin(angle_m * (i + 1) - M_PI / 2);

		ctx.newSubPath();
		ctx.setSource(Color(1, 1, 1));
		ctx.circle(cx + x, cy + y, 4);// , 0.0, 2 * M_PI);
		ctx.fill();

		ctx.newSubPath();
		ctx.setSource(ColorAf(1, 1, 1, 0.5f));
		ctx.line(vec2(cx + x, cy + y), vec2(cx + x, screenHeight_m));
		ctx.stroke();

		for (size_t j = 0; j < rows_m; j++)
		{
			curves_m[i + j * cols_m]->setX(cx + x);
		}
	}

	for (size_t j = 0; j < rows_m; j++)
	{
		float cy = w_m + j * w_m + w_m / 2;
		float cx = w_m / 2;
		ctx.newSubPath();
		ctx.setSource(clr);
		//ctx.scale(1, 2);
		ctx.circle(cx, cy, radius);// , 0.0, 2 * M_PI);
		ctx.stroke();

		float x = radius * cos(angle_m * (j + 1) - M_PI / 2);
		float y = radius * sin(angle_m * (j + 1) - M_PI / 2);

		ctx.newSubPath();
		ctx.setSource(Color(1, 1, 1));
		ctx.circle(cx + x, cy + y, 4);// , 0.0, 2 * M_PI);
		ctx.fill();

		ctx.newSubPath();
		ctx.setSource(ColorAf(1, 1, 1, 0.5f));
		ctx.line(vec2(cx + x, cy + y), vec2(screenWidth_m, cy + y));
		ctx.stroke();

		for (size_t i = 0; i < cols_m; i++)
		{
			curves_m[i + j * cols_m]->setY(cy + y);
		}	
	}



	for (size_t i = 0; i < curves_m.size(); i++)
	{
		auto tempLocation = curves_m[i]->getCurrent();
		ctx.newSubPath();
		ctx.setSource(ci::Color(1, 1, 1));
		ctx.circle(tempLocation.x, tempLocation.y, 4);
		ctx.fill();
		curves_m[i]->draw(ctx2_m);
	}

	angle_m -= .01f;

	if (angle_m < -2*M_PI )
	{
		for (size_t i = 0; i < curves_m.size(); i++)
		{
			curves_m[i]->reset();			
		}
		angle_m = 0;
	}


	gl::Texture2dRef texture = gl::Texture::create(cairoSurface.getSurface());
	gl::draw(texture);

	gl::Texture2dRef texture2 = gl::Texture::create(cairoSurface_m.getSurface());
	gl::draw(texture2);

	//params_m->draw();
}

CINDER_APP( LissajousCurneTableApp, RendererGl )


