#pragma once

#include "renderer.hpp"
#include "framebuffer.hpp"

class Director
{
private:
	Renderer *rend;
	Framebuffer *fb;
	
public:
	Director();
	virtual ~Director();
	
	void setRenderer(Renderer *r);
	Renderer *getRenderer();
	
	void setFramebuffer(Framebuffer *f);
	Framebuffer *getFramebuffer();
	
	virtual void beginSession() = 0;
	virtual void processSession() = 0;
	virtual void endSession() = 0;
	
	virtual void drawPreview(int w, int h);
};
