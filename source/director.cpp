#include "director.hpp"

Director::Director()
{
	
}

Director::~Director()
{
	
}

void Director::setRenderer(Renderer *r)
{
	rend = r;
}

Renderer *Director::getRenderer()
{
	return rend;
}


void Director::setFramebuffer(Framebuffer *f)
{
	fb = f;
}

Framebuffer *Director::getFramebuffer()
{
	return fb;
}

void Director::drawPreview(int w, int h)
{
	fb->draw(w,h);
}
