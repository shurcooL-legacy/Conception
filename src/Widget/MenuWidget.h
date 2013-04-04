#pragma once
#ifndef __MenuWidget_H__
#define __MenuWidget_H__

template <typename T> class MenuWidget
	: public Widget
{
public:
	MenuWidget(Vector2n Position, std::vector<T> & List, TypingModule * TypingModule = nullptr);
	virtual ~MenuWidget();

	const T * GetSelectedEntry() const;

	void SetSelectedEntryId(Vector2n LocalPosition);

	void Render() override;

	void ProcessTap(const InputEvent & InputEvent, Vector2n Position) override;
	void ProcessDoubleTap(const InputEvent & InputEvent, Vector2n Position) override;

	void ProcessEvent(InputEvent & InputEvent) override;

	std::function<void()>								m_OnChange = nullptr;
	std::function<void(Vector2n, std::vector<T> &)>		m_TapAction = nullptr;
	std::function<void(Vector2n, std::vector<T> &)>		m_DoubleTapAction = nullptr;		// TODO: Inconsistency, this is here while GR.RecDblTap is off

private:
	void UpdateDimensions();

	std::vector<T>				m_Entries;

	typename decltype(m_Entries)::size_type		m_SelectedEntryId = -1;

	const Color		m_SelectedColor = Color(0.3529, 0.5686, 0.8235);
	const Color		m_UnfocusedSelectedColor = Color(static_cast<uint8>(212), 212, 212);

	TypingModule * m_TypingModule;

public:
	void SetSelectedEntryId(decltype(m_SelectedEntryId) SelectedEntryId);
};

#include "MenuWidget.hpp"

#endif // __MenuWidget_H__
