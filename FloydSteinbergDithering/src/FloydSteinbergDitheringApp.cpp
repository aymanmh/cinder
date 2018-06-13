#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FloydSteinbergDitheringApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
	void dither();
	int windowWidth_m = 1024;
	int windowHeight_m = 512;
	gl::Texture2dRef orgTexture_m;
	gl::Texture2dRef newTexture_m;
	Surface32fRef imageSurface_m;
};

void FloydSteinbergDitheringApp::setup()
{
	getWindow()->setTitle("Floyd-Steinberg Dithering");
	setWindowSize(windowWidth_m, windowHeight_m);
	setFrameRate(10.0f);

	//Surface8u img = loadImage(loadAsset("kitten.jpg"));
	Channel8u img = loadImage(loadAsset("kitten.jpg"));
	orgTexture_m = gl::Texture2d::create(img);
	imageSurface_m = std::make_shared<Surface32f>(img);

	dither();

	newTexture_m = gl::Texture2d::create(*imageSurface_m);
}

void FloydSteinbergDitheringApp::mouseDown( MouseEvent event )
{
}

void FloydSteinbergDitheringApp::update()
{

}

void FloydSteinbergDitheringApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::draw(orgTexture_m,vec2(0.0f,0.0f));
	gl::draw(newTexture_m,vec2(512.0f,0.0f));
}

void FloydSteinbergDitheringApp::dither()
{
	Area area(0, 0, imageSurface_m->getWidth(), imageSurface_m->getHeight());
	int factor = 1;
	Surface32f::Iter outIter(imageSurface_m->getIter(area));
	while (outIter.line())
	{
		while (outIter.pixel())
		{
			float oldR = outIter.r();
			float oldG = outIter.g();
			float oldB = outIter.b();
			float newR = ( round(factor * oldR ) * (255 / factor) ) / 255.0f;
			float newG = ( round(factor * oldG ) * (255 / factor) ) / 255.0f;
			float newB = ( round(factor * oldB ) * (255 / factor) ) / 255.0f;
			outIter.r() = newR;
			outIter.g() = newG;
			outIter.b() = newB;
			float errorR, errorG, errorB, r, g, b;
			ivec2 pixelPosition;
			ColorA pixelColor;
			ColorA newPixelColor;

			errorR = oldR - newR;
			errorG = oldG - newG;
			errorB = oldB - newB;

			if (outIter.x() + 1 <= outIter.getWidth())
			{
				pixelPosition = ivec2(outIter.x() + 1, outIter.y());
				pixelColor = imageSurface_m->getPixel(pixelPosition);

				r = pixelColor.r;
				g = pixelColor.g;
				b = pixelColor.b;

				r += errorR * 7.0f / 16.0f;
				g += errorG * 7.0f / 16.0f;
				b += errorB * 7.0f / 16.0f;
				newPixelColor = ColorA(r, g, b);
				
				imageSurface_m->setPixel(pixelPosition, newPixelColor);
			}

			if (outIter.x() - 1 >= 0 && outIter.y() + 1 <= outIter.getHeight())
			{
				pixelPosition = ivec2(outIter.x() - 1, outIter.y() + 1);
				pixelColor = imageSurface_m->getPixel(pixelPosition);

				r = pixelColor.r;
				g = pixelColor.g;
				b = pixelColor.b;

				r += errorR * 3.0f / 16.0f;
				g += errorG * 3.0f / 16.0f;
				b += errorB * 3.0f / 16.0f;
				newPixelColor = ColorA(r, g, b);

				imageSurface_m->setPixel(pixelPosition, newPixelColor);
			}

			if (outIter.y() + 1 <= outIter.getHeight())
			{
				pixelPosition = ivec2(outIter.x(), outIter.y() + 1);
				pixelColor = imageSurface_m->getPixel(pixelPosition);

				r = pixelColor.r;
				g = pixelColor.g;
				b = pixelColor.b;

				r += errorR * 5.0f / 16.0f;
				g += errorG * 5.0f / 16.0f;
				b += errorB * 5.0f / 16.0f;
				newPixelColor = ColorA(r, g, b);

				imageSurface_m->setPixel(pixelPosition, newPixelColor);
			}

			if (outIter.x() + 1 <= outIter.getWidth() && outIter.y() + 1 <= outIter.getHeight())
			{
				pixelPosition = ivec2(outIter.x() + 1, outIter.y() + 1);
				pixelColor = imageSurface_m->getPixel(pixelPosition);

				r = pixelColor.r;
				g = pixelColor.g;
				b = pixelColor.b;

				r += errorR * 1.0f / 16.0f;
				g += errorG * 1.0f / 16.0f;
				b += errorB * 1.0f / 16.0f;
				newPixelColor = ColorA(r, g, b);

				imageSurface_m->setPixel(pixelPosition, newPixelColor);
			}
		}
	}
}

CINDER_APP( FloydSteinbergDitheringApp, RendererGl )
