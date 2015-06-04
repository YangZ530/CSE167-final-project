#ifndef _SHADER_H_
#define _SHADER_H_

#include "GLee.h"

/*! Handles GLSL shaders.  It can load the code from a file or read straight
* from a char array. */
class Shader
{

private:

	char* read(const char *filename);
	void setup(const char *vs, const char *fs);

protected:

	GLhandleARB pid;
	static GLhandleARB currentlyBoundShaderID;

public:

	Shader(const char *vert, const char *frag, bool isFile = true);
	~Shader();

	void bind();
	void unbind();

	GLhandleARB getPid();

	void printLog(const char* tag = "");

};

#endif
