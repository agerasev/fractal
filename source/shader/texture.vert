attribute vec2 aShape;
attribute vec2 aTexCoord;

void main(void) 
{
	gl_TexCoord[0] = vec4(aTexCoord,0.0,1.0);
	gl_Position = vec4(aShape,0.0,1.0);
}
