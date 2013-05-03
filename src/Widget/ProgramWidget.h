#pragma once
#ifndef __ProgramWidget_H__
#define __ProgramWidget_H__

class ProgramWidget
	: public FlowLayoutWidget
{
public:
	ProgramWidget(Vector2n Position, TypingModule & TypingModule, Project & Project, TextFieldWidget * Target = nullptr);
	~ProgramWidget();

	void SetTarget(TextFieldWidget * Target);

	void ProcessTimePassed(const double TimePassed) override;

	ConnectionWidget<TextFieldWidget> * m_SourceWidget;
	ConnectionWidget<TextFieldWidget> * m_StdinWidget;
	TextFieldWidget * m_OutputWidget;

	std::function<std::string(uint32)>		m_GetLineAnnotations = nullptr;

private:
	ProgramWidget(const ProgramWidget &) = delete;
	ProgramWidget & operator = (const ProgramWidget &) = delete;

	Project & m_Project;
};

#endif // __ProgramWidget_H__
