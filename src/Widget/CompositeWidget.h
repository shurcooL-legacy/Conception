#pragma once
#ifndef __CompositeWidget_H__
#define __CompositeWidget_H__

class CompositeWidget
	: public Widget
{
public:
	CompositeWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets);
	~CompositeWidget();

	void Render() override;

	bool HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const override;

//private:
	std::vector<std::shared_ptr<Widget>>		m_Widgets;
};

#endif // __CompositeWidget_H__
