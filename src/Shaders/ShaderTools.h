#pragma once
#ifndef __ShaderTools_H__
#define __ShaderTools_H__

#if 1
#define CHECK_GL_ERRORS { GLenum err = glGetError(); if (err) printf( "Error %x at line %d of file %s\n", err, __LINE__, __FILE__); }
#else
#define CHECK_GL_ERRORS {}
#endif

class ShaderTools
{
public:
	ShaderTools(InputManager & InputManager);
	~ShaderTools();

	void ReloadShaders();

private:
	ShaderTools(const ShaderTools &);
	ShaderTools & operator =(const ShaderTools &);

	void BuildShaders();
	void DestroyShaders();

	public:GLuint		m_MotionBlurFboId;
	GLuint		m_MotionBlurColorTexId;
	GLsizei		m_MotionBlurImageWidth;
	GLsizei		m_MotionBlurImageHeight;

	GLSLProgramObject	m_MotionBlurShader;private:
};

#endif // __ShaderTools_H__
