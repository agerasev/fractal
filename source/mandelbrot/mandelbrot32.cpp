#include "mandelbrot32.hpp"

#include "shader/mandel32.h"

Mandelbrot32::Mandelbrot32() : 
	Mandelbrot(MAIN_VERT_SHADER_SOURCE,MANDEL32_SHADER_SOURCE)
{
	position_uniform = glGetUniformLocation(program,"uPosition");
	factor_uniform = glGetUniformLocation(program,"uFactor");
	depth_uniform = glGetUniformLocation(program,"uDepth");
	samples_uniform = glGetUniformLocation(program,"uSamples");
}

Mandelbrot32::~Mandelbrot32()
{
	
}

void Mandelbrot32::render()
{
	float p[2];
	p[0] = position.re();
	p[1] = position.im();
	
	float f[2];
	f[0] = factor.re();
	f[1] = factor.im();
	
	float aspect[2] = {width/2.0f,height/2.0f};
	
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	{
		glUniform2fv(aspect_uniform, 1, aspect);
		glUniform4fv(viewport_uniform, 1, viewport);
		
		glUniform2fv(position_uniform, 1, p);
		glUniform2fv(factor_uniform, 1, f);
		glUniform1i(depth_uniform, depth);
		glUniform1i(samples_uniform, samples);
		
		glEnableVertexAttribArray(shape_attrib);
		{
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glVertexAttribPointer(shape_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		glDisableVertexAttribArray(shape_attrib);
	}
	glUseProgram(0);
}
