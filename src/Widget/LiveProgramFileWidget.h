#pragma once
#ifndef __LiveProgramFileWidget_H__
#define __LiveProgramFileWidget_H__

class LiveProgramFileWidget
	: public FlowLayoutWidget
{
public:
	LiveProgramFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule, Project & Project);
	~LiveProgramFileWidget();

private:
	LiveProgramFileWidget(const LiveProgramFileWidget &) = delete;
	LiveProgramFileWidget & operator = (const LiveProgramFileWidget &) = delete;

	TextFileWidget * m_SourceFileWidget;
	TextFieldWidget * m_SourceWidget;		// This points to m_SourceFileWidget->m_TextFieldWidget
	ProgramWidget * m_ProgramWidget;
};

#endif // __LiveProgramFileWidget_H__
