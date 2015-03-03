#include "renderer.hpp"

#include <media/media.h>

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

static void printProgramLinkingErrors(GLuint id, const char *name)
{
	int link_ok;
	glGetProgramiv(id, GL_LINK_STATUS, &link_ok);
	if(!link_ok) {
		printWarn("%s: Linking error",name);
	}
}

static const char *VERTEX_SHADER_SOURSE = 
	"attribute vec2 aShape;\n"
	"void main(void){\n"
		"gl_Position = vec4(aShape,0.0,1.0);\n"
	"}\n";

static const char *FRAGMENT_SHADER_SOURSE = 
	"uniform vec2 uPosition;\n"
	"uniform vec2 uFactor;\n"
	"void main(void){\n"
		"gl_FragColor = vec4(0.0,1.0,0.0,1.0);\n"
	"}\n";

Renderer::Renderer()
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
	glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SOURSE, NULL);
	glCompileShader(vertex_shader);
	printShaderCompilationErrors(vertex_shader,"Vertex Shader");
	
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SOURSE, NULL);
	glCompileShader(fragment_shader);
	printShaderCompilationErrors(fragment_shader,"Fragment Shader");
	
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	
	glLinkProgram(program);
	
	position_uniform = glGetUniformLocation(program,"uPosition");
	factor_uniform = glGetUniformLocation(program,"uFactor");
	shape_attrib = glGetAttribLocation(program,"aShape");
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
    glViewport(0,0,w,h);
}

void Renderer::transform(creal p, creal f)
{
    
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	{
		cfloat p = position, f = factor;
		glUniform2fv(position_uniform, 1, p.data);
		glUniform2fv(factor_uniform, 1, f.data);
		
		glEnableVertexAttribArray(shape_attrib);
		{
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glVertexAttribPointer(shape_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		glDisableVertexAttribArray(shape_attrib);
	}
	glUseProgram(0);
}
