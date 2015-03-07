#include "mandelbrot128.hpp"

#include "shader/mandel128.h"

Mandelbrot128::Mandelbrot128() : 
	Mandelbrot(MAIN_VERT_SHADER_SOURCE,MANDEL128_SHADER_SOURCE)
{
	position_re_uniform = glGetUniformLocation(program,"uPositionRe");
	position_im_uniform = glGetUniformLocation(program,"uPositionIm");
	
	factor_re_uniform = glGetUniformLocation(program,"uFactorRe");
	factor_im_uniform = glGetUniformLocation(program,"uFactorIm");
	
	depth_uniform = glGetUniformLocation(program,"uDepth");
	samples_uniform = glGetUniformLocation(program,"uSamples");
}

Mandelbrot128::~Mandelbrot128()
{
	
}

void Mandelbrot128::render()
{
	double dp[4];
	dp[0] = position.re();
	dp[2] = position.im();
	dp[1] = position.re() - dp[0];
	dp[3] = position.im() - dp[2];
	
	float p[8];
	p[0] = dp[0];
	p[2] = dp[1];
	p[4] = dp[2];
	p[6] = dp[3];
	p[1] = dp[0] - p[0];
	p[3] = dp[1] - p[2];
	p[5] = dp[2] - p[4];
	p[7] = dp[3] - p[6];
	
	double df[4];
	df[0] = factor.re();
	df[2] = factor.im();
	df[1] = factor.re() - df[0];
	df[3] = factor.im() - df[2];
	
	float f[8];
	f[0] = df[0];
	f[2] = df[1];
	f[4] = df[2];
	f[6] = df[3];
	f[1] = df[0] - f[0];
	f[3] = df[1] - f[2];
	f[5] = df[2] - f[4];
	f[7] = df[3] - f[6];
	
	float aspect[2] = {width/2.0f,height/2.0f};
	
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	{
		glUniform2fv(aspect_uniform, 1, aspect);
		glUniform4fv(viewport_uniform, 1, viewport);
		
		glUniform4fv(position_re_uniform, 1, p+0);
		glUniform4fv(position_im_uniform, 1, p+4);
		
		glUniform4fv(factor_re_uniform, 1, f+0);
		glUniform4fv(factor_im_uniform, 1, f+4);
		
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
