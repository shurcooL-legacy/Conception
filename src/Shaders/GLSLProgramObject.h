//
// GLSLProgramObject.h - Wrapper for GLSL program objects
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#ifndef GLSL_PROGRAM_OBJECT_H
#define GLSL_PROGRAM_OBJECT_H

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>

class GLSLProgramObject
{
public:
	GLSLProgramObject();
	virtual ~GLSLProgramObject();

	void destroy();

	void bind();

	void unbind();

	void setUniform(std::string name, GLfloat* val, int count);
	void setUniform(std::string name, GLint* val, int count);

	void setTextureUnit(std::string texname, int texunit);

	void bindTexture(GLenum target, std::string texname, GLuint texid, int texunit);

	void bindTexture2D(std::string texname, GLuint texid, int texunit) {
		bindTexture(GL_TEXTURE_2D, texname, texid, texunit);
	}

	void bindTexture3D(std::string texname, GLuint texid, int texunit) {
		bindTexture(GL_TEXTURE_3D, texname, texid, texunit);
	}

	void bindTextureRECT(std::string texname, GLuint texid, int texunit) {
		bindTexture(GL_TEXTURE_RECTANGLE, texname, texid, texunit);
	}

	void attachVertexShader(std::string filename);

	void attachFragmentShader(std::string filename);

	void link();

	inline GLuint getProgId() { return _progId; }

protected:
	std::vector<GLuint>		_vertexShaders;
	std::vector<GLuint>		_fragmentShaders;
	GLuint					_progId;
};

#endif
