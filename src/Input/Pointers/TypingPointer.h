#pragma once
#ifndef __TypingPointer_H__
#define __TypingPointer_H__

class TypingPointer
	: public Pointer
{
public:
	TypingPointer(InputHandler & InputHandler);
	virtual ~TypingPointer();

	/*void ProcessButton(Input::InputId ButtonId, bool Pressed);

	void ProcessCharacter(int Character);*/

	DeviceCategory GetDeviceCategory() const override;
	VirtualCategory GetVirtualCategory() const override;

private:
	TypingPointer(const TypingPointer &);
	TypingPointer & operator =(const TypingPointer &);

	//std::vector<bool>		m_ButtonsPressed;
};

#endif // __TypingPointer_H__
