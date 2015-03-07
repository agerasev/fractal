#include <media/media.h>

#include "renderer.hpp"

static void printShaderCompilationErrors(GLuint id, const char *name)
{
	int   infologLen   = 0;
	int   charsWritten = 0;
	char *infoLog;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLen);
	if(infologLen > 1)
	{
		infoLog = new char[infologLen];
		glGetShaderInfoLog(id, infologLen, &charsWritten, infoLog);
		printWarn("%s:\n%s\n",name,infoLog);
		delete infoLog;
	}
}

Renderer::Renderer(const char *vs, const char *fs) 
  : viewport{0,0,1,1}, samples(1)
{
	/* Create buffer */
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	float quad[12] = {
		1.0f,1.0f,
		-1.0f,1.0f,
		-1.0f,-1.0f,
		1.0f,1.0f,
		-1.0f,-1.0f,
		1.0f,-1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 2*3*2*sizeof(float), quad, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	/* Create program */
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vs, NULL);
	glCompileShader(vertex_shader);
	printShaderCompilationErrors(vertex_shader,"Vertex Shader");
	
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fs, NULL);
	glCompileShader(fragment_shader);
	printShaderCompilationErrors(fragment_shader,"Fragment Shader");
	
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	
	glLinkProgram(program);
	
	shape_attrib = glGetAttribLocation(program,"aShape");
	aspect_uniform = glGetUniformLocation(program,"uAspect");
	viewport_uniform = glGetUniformLocation(program,"uViewport");
}

Renderer::~Renderer()
{
	/* Delete buffer */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &buffer);
	
	/* Delete program */
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	glDeleteProgram(program);
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Renderer::resize(int w, int h)
{
	width = w;
	height = h;
	
	glViewport(0,0,width,height);
}

void Renderer::setSamples(int s)
{
	samples = s;
}

void Renderer::setViewport(float x0, float y0, float x1, float y1)
{
	viewport[0] = x0;
	viewport[1] = y0;
	viewport[2] = x1;
	viewport[3] = y1;
}

void Renderer::transform(creal p, creal f)
{
    position = p;
    factor = f;
}

creal Renderer::getPosition() const
{
    return position;
}

creal Renderer::getFactor() const
{
    return factor;
}
