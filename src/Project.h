#pragma once
#ifndef __Project_H__
#define __Project_H__

class Project
{
public:
	Project();
	~Project();

	void LoadSampleGenProgram(CanvasWidget & CanvasTEST);

	void SetStdinContent(ConnectionWidget<TextFieldWidget> * StdinSource);
	std::string GetStdinContent();

	void GenerateProgram(const std::string & ProgramContent);
	void GenerateProgramForFunction(std::ostream & Out, const std::string & InputContent, const std::string & FunctionContent, std::vector<ConceptId> & Imports);
	void RunProgram(TextFieldWidget * OutputWidget);

	std::vector<ConceptId> & GetStdIncludes() { return std_includes; }

	void StartBackgroundThread();

private:
	Project(const Project &);
	Project & operator = (const Project &);

	std::vector<ConceptId> std_includes;
	std::vector<Function> functions2;

public:volatile uint8	m_BackgroundState;private:
	volatile pid_t	m_LastPid;
	volatile double m_ProcessStartedTime;
public:volatile double m_ProcessEndedTime;private:
	volatile bool	m_ExpiredOutput;
	int				m_PipeFd[2];
	int				m_PipeInFd[2];
	Thread			m_BackgroundThread;

	ConnectionWidget<TextFieldWidget> * m_StdinSource = nullptr;

	const Color		m_CompilingColor = Color(0.98, 0.98, 0.98);
	const Color		m_RunningColor = Color(1.0, 1, 1);
	const Color		m_ErrorCompileColor = Color(1.0, 0.9, 0.9);
	const Color		m_FinishedSuccessColor = Color(0.9, 1, 0.9);
	const Color		m_FinishedErrorColor = Color(0.9, 0.9, 1);

	static void GLFWCALL BackgroundThread(void * Argument);

	friend class ProgramWidget;
};

#endif // __Project_H__
