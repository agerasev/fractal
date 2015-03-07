#include "videodirector.hpp"

#include <media/media.h>

#include <cmath>

VideoDirector::VideoDirector()
{
	
}

VideoDirector::~VideoDirector()
{
	
}

void VideoDirector::addTransition(Transition t)
{
	path.push_back(t);
}

void VideoDirector::interpolate(const Transition &tr, real t)
{
	creal a = tr.begin.factor, b = tr.end.factor;
	creal c = tr.begin.position, d = tr.end.position;
	
	real ar = a.mod(), br = b.mod();
	real aa = a.arg(), ba = b.arg();
	
	real mod = ar*pow(br/ar,t);
	real arg = (ba - aa + M_PIl)/(2.0*M_PIl);
	arg -= floor(arg);
	arg = aa + (arg*(2.0*M_PIl) - M_PIl)*t;
	
	getRenderer()->transform(
	  c + (d - c)*(mod - ar)/(br - ar),
	  polar(mod,arg)
	);
}

void VideoDirector::beginSession()
{
	counter = 0;
	time = 0.0;
}

void VideoDirector::processSession()
{
	real new_time = time + path[floor(time)].speed;
	unsigned int index = floor(new_time);
	if(index >= path.size())
	{
		return;
	}
	time = new_time;
	real dt = time - index;
	
	interpolate(path[index],dt);
	
	getFramebuffer()->render(getRenderer());
}

void VideoDirector::endSession()
{
	
}
