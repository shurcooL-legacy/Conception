#include "../../Main.h"

TypingPointer::TypingPointer(InputHandler & InputHandler)
	: Pointer(InputHandler)
{
}

TypingPointer::~TypingPointer()
{
}

TypingPointer::DeviceCategory TypingPointer::GetDeviceCategory() const
{
	return DeviceCategory::KEYBOARD;
}

TypingPointer::VirtualCategory TypingPointer::GetVirtualCategory() const
{
	return VirtualCategory::TYPING;
}
