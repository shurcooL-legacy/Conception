#include "../../Main.h"

TypingPointer::TypingPointer(InputResponder & InputResponder)
	: Pointer(InputResponder)
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
