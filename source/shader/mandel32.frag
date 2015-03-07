#version 120

#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

uniform vec2 uPosition;
uniform vec2 uFactor;
uniform int  uDepth;
uniform int  uSamples;
uniform vec2 uAspect;

varying vec2 vDeviation;

float abs2(vec2 a) 
{
	return a.x*a.x + a.y*a.y;
}

vec2 cmul(vec2 a, vec2 b) 
{
	return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 csqr(vec2 a)
{
	return vec2(a.x*a.x - a.y*a.y, 2.0*a.x*a.y);
}

vec4 mandel_sample(vec2 c)
{
	int i;
	vec2 z = vec2(0.0,0.0);
	for(i = 0; i < uDepth && abs2(z) < 16.0; ++i) 
	{
		z = csqr(z) + c;
	}
	if(i == uDepth) 
	{
		return vec4(0.0,0.0,0.0,1.0);
	} 
	else 
	{
		float smooth_i = float(i) + 1.0 - log(log(sqrt(abs2(z))))/log(2.0);
		float yellow = -0.5*cos(smooth_i/8.0) + 0.5;
		float blue = 0.1*cos(smooth_i/8.0) + 0.1;
		return vec4(yellow,yellow,blue,1.0);
	}
}

void main(void) 
{
	vec4 color = vec4(0.0,0.0,0.0,0.0);
	float step = 1.0/float(uSamples);
	
	for(int ix = 0; ix < uSamples; ++ix)
	{
		for(int iy = 0; iy < uSamples; ++iy)
		{
			color += mandel_sample(
			  uPosition + cmul(
			    uFactor,
			    vDeviation + vec2(
			      0.5*(step-1.0) + step*float(ix),
			      0.5*(step-1.0) + step*float(iy)
			    )
			  )
			);
		}
	}

	color /= float(uSamples*uSamples);
	
	gl_FragColor = color;
}
