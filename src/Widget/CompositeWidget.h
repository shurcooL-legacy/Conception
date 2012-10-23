#pragma once
#ifndef __CompositeWidget_H__
#define __CompositeWidget_H__

class CompositeWidget
	: public Widget
{
public:
	CompositeWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors);
	CompositeWidget(Vector2n Position, Vector2n Dimensions, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors);
	~CompositeWidget();

	void AddWidget(Widget * Widget);
	void RemoveWidget(Widget * Widget);

	void Render() override;

	bool HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const override;

private:
	CompositeWidget(const CompositeWidget &) = delete;
	CompositeWidget & operator = (const CompositeWidget &) = delete;

	std::vector<std::shared_ptr<Widget>>		m_Widgets;

protected:
	const decltype(m_Widgets) & GetWidgets() const { return m_Widgets; }
};

#endif // __CompositeWidget_H__
