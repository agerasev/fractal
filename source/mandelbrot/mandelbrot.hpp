#pragma once

#include <renderer.hpp>

class Mandelbrot : public Renderer
{
protected:
	int depth = 256;
	
public:
	Mandelbrot(const char *vs, const char *fs);
	virtual ~Mandelbrot();
	
	void setDepth(int d);
};
