#include <list>

#include <media/media.h>

#include "renderer.hpp"
#include "framebuffer.hpp"
#include "director.hpp"

#include "mandelbrot/mandelbrot32.hpp"
#include "mandelbrot/mandelbrot64.hpp"
#include "mandelbrot/mandelbrot128.hpp"

#include "videodirector.hpp"

class State
{
public:
	int done = false;
	int wait = false;
	int width;
	int height;
	bool ready = false;
	
	Renderer *rend = nullptr;
	Framebuffer *fb = nullptr;
	Director *dir = nullptr;
	
	bool explore = false;
	int counter = 1;
	
	void init()
	{
		int sw, sh;
		Mandelbrot *mb;
		
		mb = new Mandelbrot64();
		rend = mb;
		
		fb = new Framebuffer();
		
		if(explore)
		{
			mb->setDepth(512);
			
			sw = 800; sh = 600;
			rend->transform(creal(-0.75,0.0),creal(0.0,-3.0/sh));
		}
		else
		{
			VideoDirector *vd;
			
			mb->setDepth(2048);
			mb->setSamples(2);
			
			sw = 1280; sh = 720;
			fb->resize(sw,sh);
			
			vd = new VideoDirector();
			dir = vd;
			
			dir->setFramebuffer(fb);
			dir->setRenderer(rend);
			
			creal pos = creal(-1.7488727620037225474192155161290429532528l,0.0000000000000000011093511439092270325842l);
			VideoDirector::Checkpoint 
			  begin = {pos,creal(0.0,-2.0/sh)},
			  end = {pos,creal(-0.0000000000000000000000000000000000000000,-0.0000000000000000005551115123125782817674)};
			
			vd->addTransition({begin,end,1.0/(30*60)});
			
			dir->beginSession();
		}
	}
	
	void term()
	{
		if(explore)
		{
			delete dir;
		}
		delete rend;
		rend = nullptr;
		delete fb;
		fb = nullptr;
	}
	
	void resize(int w, int h)
	{
		width = w;
		height = h;
		
		if(explore)
		{
			fb->resize(w,h);
		}
		
		ready = true;
	}
	
	void render()
	{
		if(explore)
		{
			if(ready && fb && fb->isExists())
			{
				printInfo(
					"p = (%0.40lf,%0.40lf)\nf = (%0.40lf,%0.40lf)\n\n",
					double(rend->getPosition().re()),
					double(rend->getPosition().im()),
					double(rend->getFactor().re()),
					double(rend->getFactor().im())
				);
				fb->render(rend);
				
				ready = false;
			}
		}
		else
		{
			dir->processSession();
			
			char num[6];
			sprintf(num,"%05d", counter);
			num[5] = '\0';
			fb->save(std::string("video/frame") + std::string(num) + std::string(".bmp"));
			
			++counter;
		}
	}
	
	void approach(int x, int y)
	{
		if(explore)
		{
			/*
			rend->transform(
				rend->getPosition() + 
				rend->getFactor()*creal(
					(real(x)/width - 0.5l)*fb->getWidth(),
					(real(y)/height - 0.5l)*fb->getHeight()
				),
				rend->getFactor()*0.5l
			);
			*/
			
			creal p = rend->getPosition();
			creal f = rend->getFactor();
			
			p.im() += 1e-16l;
			
			rend->transform(p,f);
			
			ready = true;
		}
	}
	
	void draw()
	{
		if(fb && fb->isExists())
		{
			if(explore)
			{
				fb->draw(width,height);
			}
			else
			{
				dir->drawPreview(width,height);
			}
		}
	}
};

void handleAppEvent(Media_App *app, const Media_AppEvent *event)
{
	State *state = static_cast<State*>(app->data);
	switch(event->type)
	{
	case MEDIA_APP_SAVESTATE:
		printInfo("Save state\n");
		break;
	case MEDIA_APP_QUIT:
		state->done = 1;
		printInfo("Quit\n");
		break;
	case MEDIA_APP_SHOW:
		state->wait = 0;
		printInfo("Show\n");
		break;
	case MEDIA_APP_HIDE:
		state->wait = 1;
		printInfo("Hide\n");
		break;
	default:
		break;
	}
}

void handleSurfaceEvent(Media_App *app, const Media_SurfaceEvent *event)
{
	State *state = static_cast<State*>(app->data);
	switch(event->type)
	{
	case MEDIA_SURFACE_INIT:
		printInfo("Init surface\n");
		state->init();
		break;
	case MEDIA_SURFACE_TERM:
		printInfo("Term surface\n");
		state->term();
		break;
	case MEDIA_SURFACE_RESIZE:
		printInfo("Resize surface ( %d, %d )\n",event->w,event->h);
		state->resize(event->w,event->h);
		break;
	default:
		break;
	}
}

void handleMotionEvent(Media_App *app, const Media_MotionEvent *event)
{
	State *state = static_cast<State*>(app->data);
	switch(event->action)
	{
	case MEDIA_ACTION_UP:
		//printInfo("Up\n");
		break;
	case MEDIA_ACTION_DOWN:
		//printInfo("Down\n");
		state->approach(event->x,event->y);
		break;
	case MEDIA_ACTION_MOVE:
		//printInfo("Move\n");
		break;
	default:
		break;
	}
	//printInfo("Motion ( %d, %d )\n", static_cast<int>(pos.x()), static_cast<int>(pos.y()));
}

void handleSensorEvent(Media_App *app, const Media_SensorEvent *event)
{
	switch(event->sensor)
	{
	case MEDIA_SENSOR_ACCELEROMETER:
		// printInfo("Accelerometer ( %f, %f, %f)\n",event->x,event->y,event->z);
		break;
	default:
		break;
	}
}

void render(Media_App *app)
{
	State *state = static_cast<State*>(app->data);
	state->draw();
}

int Media_main(Media_App *app)
{
	State state;
	
	app->data = static_cast<void*>(&state);

	app->listeners.app = &handleAppEvent;
	app->listeners.surface = &handleSurfaceEvent;
	app->listeners.motion = &handleMotionEvent;
	app->listeners.sensor = &handleSensorEvent;

	app->renderer = &render;
	
	for(;;)
	{
		if(state.wait)
		{
			Media_waitForEvent(app);
		}
		Media_handleEvents(app);

		if(state.done)
		{
			break;
		}
		
		state.render();
		
		Media_renderFrame(app);
	}

	return 0;
}
