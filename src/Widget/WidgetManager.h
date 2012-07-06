#pragma once
#ifndef __WidgetManager_H__
#define __WidgetManager_H__

#if 0
class WidgetManager
{
public:
	WidgetManager();
	~WidgetManager();

	void Add(Widget * Widget);

	void Render(Canvas & Canvas);

	bool HitTest(Vector2d Position) const;
	void ProcessPress(Vector2d Position);
	void ProcessMove(Vector2d Position);
	void ProcessRelease(Vector2d Position);
	void ProcessDeactivation();

private:
	WidgetManager(const WidgetManager &);
	WidgetManager & operator =(const WidgetManager &);

	std::vector<Widget *>		m_Widgets;

	Widget * m_HoverWidget;
	Widget * m_ActiveWidget;

	friend Widget;
};
#endif

#endif // __WidgetManager_H__
