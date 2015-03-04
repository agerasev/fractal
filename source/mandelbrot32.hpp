#pragma once

#include "renderer.hpp"

class Mandelbrot32 : public Renderer
{
protected:
	GLint position_uniform;
	GLint factor_uniform;
	GLint depth_uniform;
	GLint samples_uniform;
	
public:
	Mandelbrot32();
	
	virtual ~Mandelbrot32();
	
	virtual void render() override;
};
