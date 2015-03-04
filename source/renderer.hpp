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
	
	int width, height;
	
	creal position;
	creal factor;
    
public:
	Renderer(const char *vs, const char *fs);
	virtual ~Renderer();
	
	virtual void resize(int w, int h);
	int getWidth() const;
	int getHeight() const;
	
	void transform(creal p, creal f);
	creal getPosition() const;
	creal getFactor() const;
	
	virtual void render() = 0;
};
