#pragma once
#ifndef __LiveProgramWidget_H__
#define __LiveProgramWidget_H__

class LiveProgramWidget
	: public FlowLayoutWidget
{
public:
	LiveProgramWidget(Vector2n Position, TypingModule & TypingModule, Project & Project);
	~LiveProgramWidget();

private:
	LiveProgramWidget(const LiveProgramWidget &) = delete;
	LiveProgramWidget & operator = (const LiveProgramWidget &) = delete;

	TextFieldWidget * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;

	Project & m_Project;
};

#endif // __LiveProgramWidget_H__
