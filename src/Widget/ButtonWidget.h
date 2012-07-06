#pragma once
#ifndef __ButtonWidget_H__
#define __ButtonWidget_H__

class ButtonWidget
	: public Widget
{
public:
	ButtonWidget(Vector2n Position);
	virtual ~ButtonWidget();

	void Render() override;
};

#endif // __ButtonWidget_H__
