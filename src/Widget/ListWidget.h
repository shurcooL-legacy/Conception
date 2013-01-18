#pragma once
#ifndef __ListWidget_H__
#define __ListWidget_H__

template <typename T> class ListWidget
#if DECISION_LIST_WIDGET_IS_COMPOSITE
	: public CompositeWidget
#else
	: public Widget
#endif
{
public:
	ListWidget(Vector2n Position, std::vector<T> & List, TypingModule & TypingModule);
	virtual ~ListWidget();

	void Insert(T & Entry);

	void Render() override;

	void ProcessTap(const InputEvent & InputEvent, Vector2n Position) override;

	std::function<void(Vector2n, std::vector<T> &)>		m_TapAction;

private:
	void UpdateDimensions();

	std::vector<T>				& m_List;

	TypingModule				& m_TypingModule;
};

#include "ListWidget.hpp"

#endif // __ListWidget_H__
