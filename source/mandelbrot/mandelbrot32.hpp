#pragma once

#include "mandelbrot.hpp"

class Mandelbrot32 : public Mandelbrot
{
protected:
	int depth = 256;
	
	GLint position_uniform;
	GLint factor_uniform;
	GLint depth_uniform;
	GLint samples_uniform;
	
public:
	Mandelbrot32();
	virtual ~Mandelbrot32();
	
	virtual void render() override;
};
