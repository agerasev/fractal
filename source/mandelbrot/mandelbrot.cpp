#include "mandelbrot.hpp"

Mandelbrot::Mandelbrot(const char *vs, const char *fs)
  : Renderer(vs,fs)
{
	
}

Mandelbrot::~Mandelbrot()
{
	
}

void Mandelbrot::setDepth(int d)
{
	depth = d;
}
