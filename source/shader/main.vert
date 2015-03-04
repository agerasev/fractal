#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

attribute vec2 aShape;

uniform vec2 uAspect;

varying vec2 vDeviation;

void main(void) 
{
	vDeviation = aShape*uAspect;
	gl_Position = vec4(aShape,0.0,1.0);
}
