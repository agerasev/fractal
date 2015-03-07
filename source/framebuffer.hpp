#pragma once

#include <string>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "renderer.hpp"

class Framebuffer
{
private:
	GLuint buffer;
	GLuint texcoord_buffer;
	
	GLuint framebuffer;
	GLuint texture;
	bool exists;
	
	GLuint program;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLint shape_attrib;
	GLint texcoord_attrib;
	GLint texture_uniform;
	
	SDL_Surface *image;
	
	int width, height;
	float viewport[4];
	
public:
	Framebuffer();
	~Framebuffer();
	
	void resize(int w, int h);
	int getWidth() const;
	int getHeight() const;
	void setViewport(float x0, float y0, float x1, float y1);
	bool isExists() const;
	
	void render(Renderer *renderer);
	
	void draw(int w, int h);
	void save(const std::string &filename);
};
