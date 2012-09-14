#pragma once
#ifndef __MotionBlurTestApp_H__
#define __MotionBlurTestApp_H__

class MotionBlurTestApp
	: public App
{
public:
	MotionBlurTestApp(InputManager & InputManager);
	~MotionBlurTestApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

	void Render() override;

	void ProcessEvent(InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

private:
	MotionBlurTestApp(const MotionBlurTestApp &);
	MotionBlurTestApp & operator = (const MotionBlurTestApp &);

	ShaderTools		m_ShaderTools;
	sint32			m_MovedTEST;
	bool			m_JustMovedTEST;
};

#endif // __MotionBlurTestApp_H__
