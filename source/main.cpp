#include <list>

#include <media/media.h>
#include "renderer.hpp"

struct State
{
	int done;
	int wait;
	int width;
	int height;
	bool ready;
	
	Renderer *rend;
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
		state->rend = new Renderer();
		break;
	case MEDIA_SURFACE_TERM:
		printInfo("Term surface\n");
		delete state->rend;
		state->rend = nullptr;
		break;
	case MEDIA_SURFACE_RESIZE:
		printInfo("Resize surface ( %d, %d )\n",event->w,event->h);
		state->width = event->w;
		state->height = event->h;
		state->rend->resize(event->w,event->h);
		state->ready = true;
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
	if(state->rend)
	{
		state->rend->render();
	}
}

int Media_main(Media_App *app)
{
	State state = {0,0,0,0,false,nullptr};
	
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

		if(state.ready)
		{
			
		}

		// printInfo("Frame\n");
		Media_renderFrame(app);
	}

	return 0;
}
