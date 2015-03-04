#include "mandelbrot64.hpp"

#include "shader/mandel64.h"

Mandelbrot64::Mandelbrot64() : 
	Renderer(MAIN_VERT_SHADER_SOURCE,MANDEL64_SHADER_SOURCE)
{
	position_uniform = glGetUniformLocation(program,"uPosition");
	factor_uniform = glGetUniformLocation(program,"uFactor");
	depth_uniform = glGetUniformLocation(program,"uDepth");
	samples_uniform = glGetUniformLocation(program,"uSamples");
}

Mandelbrot64::~Mandelbrot64()
{
	
}

void Mandelbrot64::render()
{
	float p[4];
	p[0] = position.re();
	p[2] = position.im();
	p[1] = position.re() - p[0];
	p[3] = position.im() - p[2];
	
	float f[4];
	f[0] = factor.re();
	f[2] = factor.im();
	f[1] = factor.re() - f[0];
	f[3] = factor.im() - f[2];
	
	float aspect[2] = {width/2.0f,height/2.0f};
	
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	{
		glUniform2fv(aspect_uniform, 1, aspect);
		glUniform4fv(position_uniform, 1, p);
		glUniform4fv(factor_uniform, 1, f);
		glUniform1i(depth_uniform, 256);
		glUniform1i(samples_uniform, 2);
		
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

