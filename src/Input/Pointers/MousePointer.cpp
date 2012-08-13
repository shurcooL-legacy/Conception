#include "../../Main.h"

MousePointer::MousePointer(InputHandler & InputHandler)
	: Pointer(InputHandler)
{
}

MousePointer::~MousePointer()
{
}

Pointer::DeviceCategory MousePointer::GetDeviceCategory() const
{
	return DeviceCategory::MOUSE;
}

MousePointer::VirtualCategory MousePointer::GetVirtualCategory() const
{
	return VirtualCategory::POINTING;
}
