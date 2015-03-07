#pragma once

#include "mandelbrot.hpp"

class Mandelbrot64 : public Mandelbrot
{
protected:
	GLint position_uniform;
	GLint factor_uniform;
	GLint depth_uniform;
	GLint samples_uniform;
	
public:
	Mandelbrot64();
	virtual ~Mandelbrot64();
	
	virtual void render() override;
};
