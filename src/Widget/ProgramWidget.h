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

private:
	ProgramWidget(const ProgramWidget &) = delete;
	ProgramWidget & operator = (const ProgramWidget &) = delete;

	ConnectionWidget<TextFieldWidget> * m_SourceWidget;
public:TextFieldWidget * m_OutputWidget;private:

	Project & m_Project;
};

#endif // __ProgramWidget_H__
