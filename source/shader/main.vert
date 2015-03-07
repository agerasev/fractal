#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

attribute vec2 aShape;

uniform vec2 uAspect;
uniform vec4 uViewport;

varying vec2 vDeviation;

void main(void) 
{
	vec4 viewport = 2.0*uViewport - vec4(1.0,1.0,1.0,1.0);
	vec2 shape = 0.5*viewport.xy*(vec2(1.0,1.0) - aShape) + 0.5*viewport.zw*(aShape + vec2(1.0,1.0));
	vDeviation = shape*uAspect;
	gl_Position = vec4(shape,0.0,1.0);
}
