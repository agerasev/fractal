#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

uniform vec4 uPosition;
uniform vec4 uFactor;
uniform int  uDepth;
uniform int  uSamples;
uniform vec2 uAspect;

varying vec2 vDeviation;

vec2 ds(float a)
{
	return vec2(a,0.0);
}

vec2 ds_add(vec2 dsa, vec2 dsb) 
{
	vec2 dsc;
	float t1, t2, e;
	t1 = dsa.x + dsb.x;
	e = t1 - dsa.x;
	t2 = ((dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y + dsb.y;
	dsc.x = t1 + t2;
	dsc.y = t2 - (dsc.x - t1);
	return dsc;
}

vec2 ds_mul(vec2 dsa, vec2 dsb) 
{
	vec2 dsc;
	float c11, c21, c2, e, t1, t2;
	float a1, a2, b1, b2, cona, conb, split = 8193.;
	cona = dsa.x * split;
	conb = dsb.x * split;
	a1 = cona - (cona - dsa.x);
	b1 = conb - (conb - dsb.x);
	a2 = dsa.x - a1;
	b2 = dsb.x - b1;
	c11 = dsa.x * dsb.x;
	c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));
	c2 = dsa.x * dsb.y + dsa.y * dsb.x;
	t1 = c11 + c2;
	e = t1 - c11;
	t2 = dsa.y * dsb.y + ((c2 - e) + (c11 - (t1 - e))) + c21;
	dsc.x = t1 + t2;
	dsc.y = t2 - (dsc.x - t1);
	return dsc;
}

vec4 cds(vec2 ca)
{
	return vec4(ca.x,0.0,ca.y,0.0);
}

vec4 cds_add(vec4 a, vec4 b) 
{
	return vec4(ds_add(a.xy,b.xy),ds_add(a.zw,b.zw));
}

vec4 cds_mul(vec4 a, vec4 b) 
{
	return vec4(ds_add(ds_mul(a.xy,b.xy),-ds_mul(a.zw,b.zw)),ds_add(ds_mul(a.xy,b.zw),ds_mul(a.zw,b.xy)));
}

float abs2(vec2 a) 
{
	return a.x*a.x + a.y*a.y;
}

float sqr(float a)
{
	return a*a;
}

vec2 ds_sqr(vec2 dsa)
{
	return ds_mul(dsa,dsa);
}

vec4 ds_mandel_sample(vec4 c)
{
	int i;
	vec4 z = vec4(0.0,0.0,0.0,0.0);
	vec4 zs = vec4(ds_sqr(z.xy),ds_sqr(z.zw));
	for(i = 0; i < uDepth && abs2(z.xz) < 16.0; ++i) 
	{
		z.zw = ds_add(ds_sqr(ds_add(z.xy,z.zw)),-ds_add(zs.xy,zs.zw));
    z.zw = ds_add(z.zw,c.zw);
    z.xy = ds_add(ds_add(zs.xy,-zs.zw),c.xy);
    zs.xy = ds_sqr(z.xy);
    zs.zw = ds_sqr(z.zw);
		// z = cds_add(cds_mul(z,z),c);
	}
	if(i == uDepth) 
	{
		return vec4(0.0,0.0,0.0,1.0);
	} 
	else 
	{
		float smooth_i = float(i) + 1.0 - log(log(sqrt(abs2(z.xz))))/log(2.0);
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
			color += ds_mandel_sample(
			  cds_add(uPosition,cds_mul(
			    uFactor,
			    cds(vDeviation + vec2(
			      0.5*(step-1.0) + step*float(ix),
			      0.5*(step-1.0) + step*float(iy)
			    ))
			  ))
			);
		}
	}
	
	color /= float(uSamples*uSamples);
	
	gl_FragColor = color;
}
