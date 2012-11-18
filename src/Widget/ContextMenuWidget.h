#pragma once
#ifndef __ContextMenuWidget_H__
#define __ContextMenuWidget_H__

template <typename T> class ContextMenuWidget
	: public Widget
{
public:
	ContextMenuWidget(Vector2n Position, std::vector<T> & List);
	virtual ~ContextMenuWidget();

	const T * GetSelectedEntry() const;

	void Render() override;

	void ProcessTap(const InputEvent & InputEvent, Vector2n Position) override;
	void ProcessDoubleTap(InputEvent & InputEvent, Vector2n Position) override;

	void ProcessEvent(InputEvent & InputEvent) override;

	std::function<void(Vector2n, std::vector<T> &)>		m_TapAction;

private:
	void UpdateDimensions();

	std::vector<T>				m_Entries;

	typename decltype(m_Entries)::size_type		m_SelectedEntry;

	const Color		m_SelectedColor = Color(0.3529, 0.5686, 0.8235);
};

#include "ContextMenuWidget.hpp"

#endif // __ContextMenuWidget_H__
