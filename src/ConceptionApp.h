#pragma once
#ifndef __ConceptionApp_H__
#define __ConceptionApp_H__

class ConceptionApp
	: public App
{
public:
	ConceptionApp(InputManager & InputManager);
	~ConceptionApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

	void Render() override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	ConceptionApp(const ConceptionApp &);
	ConceptionApp & operator = (const ConceptionApp &);

	Project		m_CurrentProject;

	TypingModule	m_TypingModule;

	TextFieldWidget * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;

	volatile uint8	m_BackgroundState;
	volatile pid_t	m_LastPid;
	volatile bool	m_ExpiredOutput;
	int				m_PipeFd[2];
	Thread			m_BackgroundThread;

	static void GLFWCALL BackgroundThread(void * pArgument);
};

#endif // __ConceptionApp_H__
