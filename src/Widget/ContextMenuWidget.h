#pragma once
#ifndef __ContextMenuWidget_H__
#define __ContextMenuWidget_H__

template <typename T> class ContextMenuWidget
	: public Widget
{
public:
	ContextMenuWidget(Vector2n Position, std::vector<T> & List, TypingModule & TypingModule);
	virtual ~ContextMenuWidget();

	void Render() override;

	void ProcessTap(InputEvent & InputEvent, Vector2n Position) override;

	void ProcessEvent(InputEvent & InputEvent) override;

	std::function<void(Vector2n, std::vector<T> &)>		m_TapAction;

private:
	void UpdateDimensions();

	std::vector<T>				m_Entries;

	typename decltype(m_Entries)::size_type		m_SelectedEntry;

	TypingModule &				m_TypingModule;

	const Color		m_UnselectedColor = Color(1.0, 1, 1);
	const Color		m_SelectedColor = Color(0.5, 0.5, 1);
};

#include "ContextMenuWidget.hpp"

#endif // __ContextMenuWidget_H__
