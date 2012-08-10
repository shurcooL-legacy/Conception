#pragma once
#ifndef __ListWidget_H__
#define __ListWidget_H__

template <typename T> class ListWidget
	: public CompositeWidget
{
public:
	ListWidget(Vector2n Position, std::vector<T> & List);
	virtual ~ListWidget();

	void Insert(T & Entry);

	void Render() override;
	
	void ProcessTap() override;

	void ProcessManipulationStarted(const PointerState & PointerState) override;
	void ProcessManipulationUpdated(const PointerState & PointerState) override;
	void ProcessManipulationCompleted(const PointerState & PointerState) override;

	std::function<void()>		m_TapAction;
	
private:
	void UpdateDimensions();

	std::vector<T>				& m_List;
};

#include "ListWidget.hpp"

#endif // __ListWidget_H__
