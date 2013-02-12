#pragma once
#ifndef __MousePointer_H__
#define __MousePointer_H__

class MousePointer
	: public Pointer
{
public:
	MousePointer(InputHandler & InputHandler);
	virtual ~MousePointer();

	/*void ProcessButton(Input::InputId ButtonId, bool Pressed);
	void ProcessPosition(Vector2n Position);*/
	//bool ProcessEvent(InputEvent & InputEvent) override;

	DeviceCategory GetDeviceCategory() const override;
	VirtualCategory GetVirtualCategory() const override;

private:
	MousePointer(const MousePointer &);
	MousePointer & operator = (const MousePointer &);
};

#endif // __MousePointer_H__
