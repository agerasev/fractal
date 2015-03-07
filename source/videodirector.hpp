#pragma once

#include <vector>

#include "complex.hpp"
#include "director.hpp"

class VideoDirector : public Director
{
public:
	
	struct Checkpoint
	{
	public:
		creal position;
		creal factor;
	};
	
	struct Transition
	{
	public:
		Checkpoint begin;
		Checkpoint end;
		real speed;
	};
	
private:
	
	int counter;
	real time;
	
	std::vector<Transition> path;
	
	void interpolate(const Transition &tr, real t);
	
public:
	VideoDirector();
	virtual ~VideoDirector();
	
	void addTransition(Transition t);
	
	virtual void beginSession() override;
	virtual void processSession() override;
	virtual void endSession() override;
};
