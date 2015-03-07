#include <media/media.h>

#include "framebuffer.hpp"

#include "shader/texture.h"

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

Framebuffer::Framebuffer() 
  : exists(false), viewport{0,0,1,1}
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
	
	glGenBuffers(1, &texcoord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
	float texcoord_quad[12] = {
		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 2*3*2*sizeof(float), texcoord_quad, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	/* Create program */
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &TEXTURE_VERT_SHADER_SOURCE, NULL);
	glCompileShader(vertex_shader);
	printShaderCompilationErrors(vertex_shader,"Vertex Shader");
	
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &TEXTURE_FRAG_SHADER_SOURCE, NULL);
	glCompileShader(fragment_shader);
	printShaderCompilationErrors(fragment_shader,"Fragment Shader");
	
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	
	glLinkProgram(program);
	
	shape_attrib = glGetAttribLocation(program,"aShape");
	texcoord_attrib = glGetAttribLocation(program,"aTexCoord");
	texture_uniform = glGetUniformLocation(program,"uTexture");
}

Framebuffer::~Framebuffer()
{
	if(exists)
	{
		glDeleteFramebuffers(1,&framebuffer);
		glDeleteTextures(1,&texture);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &buffer);
	
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	glDeleteProgram(program);
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Framebuffer::resize(int w, int h)
{
	width = w;
	height = h;
	
	if(exists)
	{
		glDeleteFramebuffers(1,&framebuffer);
		glDeleteTextures(1,&texture);
		
		SDL_FreeSurface(image);
	}
	
	/* Create framebuffer */
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	glGenTextures(1, &texture);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	
	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	
	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		printWarn("Framebuffer failed");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	/* Create image */
	image = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,24,0x0000ff,0x00ff00,0xff0000,0x000000);
	
	exists = true;
}

int Framebuffer::getWidth() const
{
    return width;
}

int Framebuffer::getHeight() const
{
    return height;
}

void Framebuffer::setViewport(float x0, float y0, float x1, float y1)
{
	viewport[0] = x0;
	viewport[1] = y0;
	viewport[2] = x1;
	viewport[3] = y1;
}

bool Framebuffer::isExists() const
{
	return exists;
}

void Framebuffer::render(Renderer *renderer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	renderer->resize(width,height);
	renderer->setViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
	renderer->render();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::draw(int w, int h)
{
	glViewport(0,0,w,h);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(texture_uniform,0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glEnableVertexAttribArray(shape_attrib);
		glEnableVertexAttribArray(texcoord_attrib);
		{
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glVertexAttribPointer(shape_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
			glVertexAttribPointer(texcoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES,0,6);
		}
		glDisableVertexAttribArray(shape_attrib);
		glDisableVertexAttribArray(texcoord_attrib);
	}
	glUseProgram(0);
	
}

void Framebuffer::save(const std::string &filename)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image->pixels);
	SDL_SaveBMP(image,filename.data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
