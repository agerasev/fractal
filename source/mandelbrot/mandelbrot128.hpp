#pragma once

#include "mandelbrot.hpp"

class Mandelbrot128 : public Mandelbrot
{
protected:
	GLint position_re_uniform;
	GLint position_im_uniform;
	
	GLint factor_re_uniform;
	GLint factor_im_uniform;
	
	GLint depth_uniform;
	GLint samples_uniform;
	
public:
	Mandelbrot128();
	virtual ~Mandelbrot128();
	
	virtual void render() override;
};
