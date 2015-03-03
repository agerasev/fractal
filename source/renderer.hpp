#pragma once

#include "complex.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

class Renderer
{
private:
    GLuint buffer;
    
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLint position_uniform;
    GLint factor_uniform;
    GLint shape_attrib;
    
    creal position;
    creal factor;
    
public:
    Renderer();
    virtual ~Renderer();
    
    void resize(int w, int h);
    
    void transform(creal p, creal f);
    
    virtual void render(); // = 0;
};
