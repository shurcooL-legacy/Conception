#pragma once
#ifndef __LiveEditorApp_H__
#define __LiveEditorApp_H__

class LiveEditorApp
	: public App
{
public:
	LiveEditorApp(InputManager & InputManager);
	~LiveEditorApp();

	void UpdateWindowDimensions(Vector2n WindowDimensions) override;

	void Render() override;

	void ProcessEvent(InputEvent & InputEvent) override;

	bool ShouldRedrawRegardless() override;

	std::string GetTitle() override;

private:
	LiveEditorApp(const LiveEditorApp &);
	LiveEditorApp & operator = (const LiveEditorApp &);

	Project		m_CurrentProject;

	TypingModule	m_TypingModule;

	TextFieldWidget * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;

	volatile uint8	m_BackgroundState;
	volatile pid_t	m_LastPid;
	volatile double m_ProcessStartedTime;
	volatile double m_ProcessEndedTime;
	volatile bool	m_ExpiredOutput;
	int				m_PipeFd[2];
	Thread			m_BackgroundThread;

	const Color		m_CompilingColor = Color(0.9, 0.9, 0.9);
	const Color		m_RunningColor = Color(1.0, 1, 1);
	const Color		m_ErrorCompileColor = Color(1.0, 0.9, 0.9);
	const Color		m_FinishedSuccessColor = Color(0.9, 1, 0.9);
	const Color		m_FinishedErrorColor = Color(0.9, 0.9, 1);

	static void GLFWCALL BackgroundThread(void * pArgument);
};

#endif // __LiveEditorApp_H__
