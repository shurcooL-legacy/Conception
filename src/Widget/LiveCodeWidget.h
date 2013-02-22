#pragma once
#ifndef __LiveCodeWidget_H__
#define __LiveCodeWidget_H__

class LiveCodeWidget
	: public CompositeWidget
{
public:
	LiveCodeWidget(Vector2n Position, TypingModule & TypingModule, Project & Project);
	~LiveCodeWidget();

private:
	LiveCodeWidget(const LiveCodeWidget &) = delete;
	LiveCodeWidget & operator = (const LiveCodeWidget &) = delete;

	std::vector<ConceptId> m_Imports;

	TextFieldWidget * m_SourceWidget;
	ToggleWidget * m_ToggleWidget;
	LiveProgramWidget * m_LiveProgramWidget;
};

#endif // __LiveCodeWidget_H__
