#pragma once
#ifndef __GitDiffWidget_H__
#define __GitDiffWidget_H__

class GitDiffWidget
	: public FlowLayoutWidget
{
public:
	GitDiffWidget(Vector2n Position, TypingModule & TypingModule, TextFileWidget * Target = nullptr);
	~GitDiffWidget();

private:
	GitDiffWidget(const GitDiffWidget &) = delete;
	GitDiffWidget & operator = (const GitDiffWidget &) = delete;

	ConnectionWidget<TextFileWidget> * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;
};

#endif // __GitDiffWidget_H__
