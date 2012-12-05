#pragma once
#ifndef __Project_H__
#define __Project_H__

class Project
{
public:
	Project();
	~Project();

	void LoadSampleGenProgram(Canvas & CanvasTEST);

	void GenerateProgram(std::string ProgramContent);
	void RunProgram(TextFieldWidget * OutputWidget);

	std::vector<ConceptId> & GetStdIncludes() { return std_includes; }

	void SetSourceOnChange(TextFieldWidget & SourceWidget, TextFieldWidget & OutputWidget, Canvas * LeftCanvas = nullptr, Canvas * RightCanvas = nullptr);
	void StartBackgroundThread();
	void SomethingFromAppRenderTEST();

private:
	Project(const Project &);
	Project & operator =(const Project &);

	std::vector<ConceptId> std_includes;
	std::vector<Function> functions2;

public:volatile uint8	m_BackgroundState;private:
	volatile pid_t	m_LastPid;
	volatile double m_ProcessStartedTime;
public:volatile double m_ProcessEndedTime;private:
	volatile bool	m_ExpiredOutput;
	int				m_PipeFd[2];
	Thread			m_BackgroundThread;

	const Color		m_CompilingColor = Color(0.9, 0.9, 0.9);
	const Color		m_RunningColor = Color(1.0, 1, 1);
	const Color		m_ErrorCompileColor = Color(1.0, 0.9, 0.9);
	const Color		m_FinishedSuccessColor = Color(0.9, 1, 0.9);
	const Color		m_FinishedErrorColor = Color(0.9, 0.9, 1);

	static void GLFWCALL BackgroundThread(void * Argument);
};

#endif // __Project_H__
