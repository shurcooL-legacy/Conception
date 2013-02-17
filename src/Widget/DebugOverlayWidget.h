#pragma once
#ifndef __DebugOverlayWidget_H__
#define __DebugOverlayWidget_H__

class DebugOverlayWidget
	: public CanvasWidget
{
public:
	DebugOverlayWidget(CanvasWidget * MainCanvas = nullptr);
	~DebugOverlayWidget();

private:
	static void PrintName(std::ostringstream & out, GestureRecognizer *& i);
};

#endif // __DebugOverlayWidget_H__
