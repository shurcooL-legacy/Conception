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
	ConceptString		m_Content;

	TypingModule & m_TypingModule;
};

#endif // __ConceptStringBoxWidget_H__
