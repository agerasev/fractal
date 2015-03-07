#pragma once

#include "complex.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

class Renderer
{
protected:
	GLuint buffer;
	
	GLuint program;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLint shape_attrib;
	GLint aspect_uniform;
	GLint viewport_uniform;
	
	int width, height;
	float viewport[4];
	
	creal position;
	creal factor;
	
	int samples;
    
public:
	Renderer(const char *vs, const char *fs);
	virtual ~Renderer();
	
	virtual void resize(int w, int h);
	void setViewport(float x0, float y0, float x1, float y1);
	
	void setSamples(int s);
	
	void transform(creal p, creal f);
	creal getPosition() const;
	creal getFactor() const;
	
	virtual void render() = 0;
};
