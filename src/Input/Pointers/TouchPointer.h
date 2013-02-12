#pragma once
#ifndef __TouchPointer_H__
#define __TouchPointer_H__

class TouchPointer
	: public Pointer
{
public:
	TouchPointer(InputHandler & InputHandler);
	virtual ~TouchPointer();

	/*void ProcessButton(Input::InputId ButtonId, bool Pressed);
	void ProcessPosition(Vector2n Position);*/
	//bool ProcessEvent(InputEvent & InputEvent) override;

	DeviceCategory GetDeviceCategory() const override;
	VirtualCategory GetVirtualCategory() const override;

private:
	TouchPointer(const TouchPointer &);
	TouchPointer & operator = (const TouchPointer &);
};

#endif // __TouchPointer_H__
