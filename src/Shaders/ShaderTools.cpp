#include "../Main.h"

ShaderTools::ShaderTools(InputManager & InputManager)
{
	{
		m_MotionBlurImageWidth = InputManager.GetWindowDimensions().X() / 2;
		m_MotionBlurImageHeight = InputManager.GetWindowDimensions().Y();
		//m_MotionBlurImageWidth = std::min<uint32>(400, nViewportWidth);
		//m_MotionBlurImageHeight = m_MotionBlurImageWidth * nViewportHeight / nViewportWidth;

		glGenFramebuffers(1, &m_MotionBlurFboId);
		glGenTextures(1, &m_MotionBlurColorTexId);

		{
			glBindTexture(GL_TEXTURE_RECTANGLE, m_MotionBlurColorTexId);
			glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat BorderColor[4] = { 1, 1, 1, 1 };
			glTexParameterfv(GL_TEXTURE_RECTANGLE, GL_TEXTURE_BORDER_COLOR, BorderColor);
			glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, m_MotionBlurImageWidth, m_MotionBlurImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			glBindFramebuffer(GL_FRAMEBUFFER, m_MotionBlurFboId);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, m_MotionBlurColorTexId, 0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		CHECK_GL_ERRORS
	}

	BuildShaders();
}

ShaderTools::~ShaderTools()
{
	if (m_MotionBlurFboId) glDeleteFramebuffers(1, &m_MotionBlurFboId);
	if (m_MotionBlurColorTexId) glDeleteTextures(1, &m_MotionBlurColorTexId);
}

void ShaderTools::ReloadShaders()
{
	DestroyShaders();
	BuildShaders();
}

void ShaderTools::BuildShaders()
{
	m_MotionBlurShader.attachVertexShader("src/Shaders/motion_blur_vertex.glsl");
	m_MotionBlurShader.attachFragmentShader("src/Shaders/motion_blur_fragment.glsl");
	m_MotionBlurShader.link();

	CHECK_GL_ERRORS
}

void ShaderTools::DestroyShaders()
{
	m_MotionBlurShader.destroy();

	CHECK_GL_ERRORS
}
