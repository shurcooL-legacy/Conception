#pragma once
#ifndef __DebugOverlayWidget_H__
#define __DebugOverlayWidget_H__

class DebugOverlayWidget
	: public CanvasWidget
{
public:
	DebugOverlayWidget(CanvasWidget * MainCanvas = nullptr);
	virtual ~DebugOverlayWidget();
};

#endif // __DebugOverlayWidget_H__
