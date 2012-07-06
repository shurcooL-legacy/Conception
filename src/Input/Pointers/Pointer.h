#pragma once
#ifndef __Pointer_H__
#define __Pointer_H__

class Pointer
	: public InputResponder,
	  public MutuallyConnectable<Pointer, GestureRecognizer>
{
public:
	enum class DeviceCategory : uint8 { KEYBOARD, MOUSE, TOUCH };
	enum class VirtualCategory : uint8 { TYPING, POINTING };

	bool ProcessEvent(InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

	virtual DeviceCategory GetDeviceCategory() const = 0;
	virtual VirtualCategory GetVirtualCategory() const = 0;

	const PointerMapping & GetPointerMapping();
	PointerMapping & ModifyPointerMapping();

	const PointerState & GetPointerState();
	PointerState & ModifyPointerState();

protected:
	Pointer(InputResponder & InputResponder);
	virtual ~Pointer();

	void ProcessActivation(InputEvent & InputEvent);
	void ProcessDeactivation(InputEvent & InputEvent);

	bool IsActive();

private:
	Pointer(const Pointer &) = delete;
	Pointer & operator = (const Pointer &) = delete;

	bool				m_IsActive;
	PointerState		m_PointerState;

	InputResponder &	m_InputResponder;
	PointerMapping		m_PointerMapping;
};

#endif // __Pointer_H__
