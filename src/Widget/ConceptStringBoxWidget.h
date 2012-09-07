#pragma once
#ifndef __ConceptStringBoxWidget_H__
#define __ConceptStringBoxWidget_H__

class ConceptStringBoxWidget
	: public Widget
{
public:
	ConceptStringBoxWidget(Vector2n Position, TypingModule & TypingModule);
	virtual ~ConceptStringBoxWidget();

	void Render() override;

	void ProcessTap(InputEvent & InputEvent, Vector2n Position) override;
	void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	ConceptString					m_Content;

	ConceptString::size_type		m_CaretPosition;

	TypingModule & m_TypingModule;

	void SetCaretPosition(decltype(m_CaretPosition) CaretPosition, bool ResetSelection, bool UpdateTargetCaretColumn = true);
	void MoveCaretTry(sint32 MoveAmount, bool ResetSelection);
};

#endif // __ConceptStringBoxWidget_H__
