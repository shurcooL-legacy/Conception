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

	MatchResult MatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents) override;

	bool HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const override;

	const Vector2n GetDimensions() const override;

	void ProcessTimePassed(const double TimePassed) override;

protected:
	bool IsHit(const Vector2n ParentPosition) const override;

private:
	CompositeWidget(const CompositeWidget &) = delete;
	CompositeWidget & operator = (const CompositeWidget &) = delete;

	MatchResult InnerMatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents);

	std::vector<std::shared_ptr<Widget>>		m_Widgets;

protected:
	const decltype(m_Widgets) & GetWidgets() const { return m_Widgets; }
};

#endif // __CompositeWidget_H__
