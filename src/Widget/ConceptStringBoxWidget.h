#pragma once
#ifndef __ConceptStringBoxWidget_H__
#define __ConceptStringBoxWidget_H__

class ConceptStringBoxWidget
	: public Widget
{
public:
	ConceptStringBoxWidget(Vector2n Position);
	virtual ~ConceptStringBoxWidget();

	void Render() override;

	void ProcessTap() override;
	void ProcessCharacter(const uint32 Character) override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	ConceptString		m_Content;
};

#endif // __ConceptStringBoxWidget_H__
